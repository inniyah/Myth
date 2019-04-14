#include "game.h"

namespace entities
{
    using namespace game;

    int extraentinfosize() { return 0; }       // size in bytes of what the 2 methods below read/write... so it can be skipped by other games

    void saveextrainfo(entity &e, char *buf)
    {
    }

    void loadextrainfo(entity &e, char *buf)
    {
    }

    void writeent(entity &e, char *buf)   // write any additional data to disk (except for ET_ ents)
    {
    }

    void readent(entity &e, char *buf, int ver)     // read from disk, and init
    {
    }

#ifndef STANDALONE
    vector<extentity *> ents;

    vector<extentity *> &getents() { return ents; }

    bool mayattach(extentity &e) { return false; }
    bool attachent(extentity &e, extentity &a) { return false; }

    const char *itemname(int i)
    {
        return NULL;
#if 0
        int t = ents[i]->type;
        if(!validitem(t)) return NULL;
        return itemstats[t-I_FIRST].name;
#endif
    }

    int itemicon(int i)
    {
        return -1;
#if 0
        int t = ents[i]->type;
        if(!validitem(t)) return -1;
        return itemstats[t-I_FIRST].icon;
#endif
    }

    const char *entmdlname(int type)
    {
        static const char * const entmdlnames[MAXENTTYPES] =
        {
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
            "game/teleport", NULL, NULL,
            NULL
        };
        return entmdlnames[type];
    }

    const char *entmodel(const entity &e)
    {
        if(e.type == TELEPORT)
        {
            if(e.attr[1] > 0) return mapmodelname(e.attr[1]);
            if(e.attr[1] < 0) return NULL;
        }
        return e.type < MAXENTTYPES ? entmdlname(e.type) : NULL;
    }

    void preloadentities()
    {
        loopi(MAXENTTYPES)
        {
            const char *mdl = entmdlname(i);
            if(!mdl) continue;
            preloadmodel(mdl);
        }
        loopv(ents)
        {
            extentity &e = *ents[i];
            switch(e.type)
            {
                case TELEPORT:
                    if(e.attr[1] > 0) preloadmodel(mapmodelname(e.attr[1]));
                case JUMPPAD:
                    if(e.attr[3] > 0) preloadmapsound(e.attr[3]);
                    break;
            }
        }
    }

    void renderentities()
    {
        loopv(ents)
        {
            extentity &e = *ents[i];
            int revs = 10;
            switch(e.type)
            {
                case TELEPORT:
                    if(e.attr[1] < 0) continue;
                    break;
                default:
                    if(!e.spawned() || !validitem(e.type)) continue;
                    break;
            }
            const char *mdlname = entmodel(e);
            if(mdlname)
            {
                vec3 p = e.o;
                p.z += 1+sinf(lastmillis/100.0+e.o.x+e.o.y)/20;
                rendermodel(mdlname, ANIM_MAPMODEL|ANIM_LOOP, p, lastmillis/(float)revs, 0, 0, MDL_CULL_VFC | MDL_CULL_DIST | MDL_CULL_OCCLUDED);
            }
        }
    }

    void addammo(int type, int &v, bool local)
    {
#if 0
        itemstat &is = itemstats[type-I_FIRST];
        v += is.add;
        if(v>is.max) v = is.max;
        if(local) msgsound(is.sound);
#endif
    }

    // these two functions are called when the server acknowledges that you really
    // picked up the item (in multiplayer someone may grab it before you).

    void pickupeffects(int n, gameent *d)
    {
#if 0
        if(!ents.inrange(n)) return;
        int type = ents[n]->type;
        if(!validitem(type)) return;
        ents[n]->clearspawned();
        if(!d) return;
        itemstat &is = itemstats[type-I_FIRST];
        if(d!=player1 || isthirdperson())
        {
            //particle_text(d->abovehead(), is.name, PART_TEXT, 2000, 0xFFC864, 4.0f, -8);
            particle_icon(d->abovehead(), is.icon%4, is.icon/4, PART_HUD_ICON_GREY, 2000, 0xFFFFFF, 2.0f, -8);
        }
        playsound(itemstats[type-I_FIRST].sound, d!=player1 ? &d->o : NULL, NULL, 0, 0, 0, -1, 0, 1500);
        d->pickup(type);
        if(d==player1) switch(type)
        {
        }
#endif
    }

    // these functions are called when the client touches the item

    void teleporteffects(gameent *d, int tp, int td, bool local)
    {
        if(ents.inrange(tp) && ents[tp]->type == TELEPORT)
        {
            extentity &e = *ents[tp];
            if(e.attr[3] >= 0)
            {
                int snd = S_TELEPORT, flags = 0;
                if(e.attr[3] > 0) { snd = e.attr[3]; flags = SND_MAP; }
                if(d == player1) playsound(snd, NULL, NULL, flags);
                else
                {
                    playsound(snd, &e.o, NULL, flags);
                    if(ents.inrange(td) && ents[td]->type == TELEDEST) playsound(snd, &ents[td]->o, NULL, flags);
                }
            }
        }
        if(local && d->clientnum >= 0)
        {
            sendposition(d);
            packetbuf p(32, ENET_PACKET_FLAG_RELIABLE);
            putint(p, N_TELEPORT);
            putint(p, d->clientnum);
            putint(p, tp);
            putint(p, td);
            sendclientpacket(p.finalize(), 0);
            flushclient();
        }
    }

    void jumppadeffects(gameent *d, int jp, bool local)
    {
        if(ents.inrange(jp) && ents[jp]->type == JUMPPAD)
        {
            extentity &e = *ents[jp];
            if(e.attr[3] >= 0)
            {
                int snd = S_JUMPPAD, flags = 0;
                if(e.attr[3] > 0) { snd = e.attr[3]; flags = SND_MAP; }
                if(d == player1) playsound(snd, NULL, NULL, flags);
                else playsound(snd, &e.o, NULL, flags);
            }
        }
        if(local && d->clientnum >= 0)
        {
            sendposition(d);
            packetbuf p(16, ENET_PACKET_FLAG_RELIABLE);
            putint(p, N_JUMPPAD);
            putint(p, d->clientnum);
            putint(p, jp);
            sendclientpacket(p.finalize(), 0);
            flushclient();
        }
    }

    void teleport(int n, gameent *d)     // also used by monsters
    {
        int e = -1, tag = ents[n]->attr[0], beenhere = -1;
        for(;;)
        {
            e = findentity(TELEDEST, e+1);
            if(e==beenhere || e<0) { conoutf(CON_WARN, "no teleport destination for tag %d", tag); return; }
            if(beenhere<0) beenhere = e;
            if(ents[e]->attr[1]==tag)
            {
                teleporteffects(d, n, e, true);
                d->o = ents[e]->o;
                d->yaw = ents[e]->attr[0];
                if(ents[e]->attr[2] > 0)
                {
                    vec3 dir;
                    vecfromyawpitch(d->yaw, 0, 1, 0, dir);
                    float speed = d->vel.magnitude2();
                    d->vel.x = dir.x*speed;
                    d->vel.y = dir.y*speed;
                }
                else d->vel = vec3(0, 0, 0);
                entinmap(d);
                updatedynentcache(d);
                ai::inferwaypoints(d, ents[n]->o, ents[e]->o, 16.f);
                break;
            }
        }
    }

    VARR(teleteam, 0, 1, 1);

    void trypickup(int n, gameent *d)
    {
        switch(ents[n]->type)
        {
            default:
                if(d->canpickup(ents[n]->type))
                {
                    addmsg(N_ITEMPICKUP, "rci", d, n);
                    ents[n]->clearspawned(); // even if someone else gets it first
                }
                break;

            case TELEPORT:
            {
                if(d->lastpickup==ents[n]->type && lastmillis-d->lastpickupmillis<500) break;
                if(!teleteam && m_teammode) break;
                if(ents[n]->attr[2] > 0)
                {
                    defformatstring(hookname, "can_teleport_%d", ents[n]->attr[2]);
                    if(!execidentbool(hookname, true)) break;
                }
                d->lastpickup = ents[n]->type;
                d->lastpickupmillis = lastmillis;
                teleport(n, d);
                break;
            }

            case JUMPPAD:
            {
                if(d->lastpickup==ents[n]->type && lastmillis-d->lastpickupmillis<300) break;
                d->lastpickup = ents[n]->type;
                d->lastpickupmillis = lastmillis;
                jumppadeffects(d, n, true);
                if(d->ai) d->ai->becareful = true;
                d->falling = vec3(0, 0, 0);
                d->physstate = PHYS_FALL;
                d->timeinair = 1;
                d->vel = vec3(ents[n]->attr[2]*10.0f, ents[n]->attr[1]*10.0f, ents[n]->attr[0]*12.5f);
                break;
            }
        }
    }

    void checkitems(gameent *d)
    {
        if(d->state!=CS_ALIVE) return;
        vec3 o = d->feetpos();
        loopv(ents)
        {
            extentity &e = *ents[i];
            if(e.type==NOTUSED) continue;
            if(!e.spawned() && e.type!=TELEPORT && e.type!=JUMPPAD) continue;
            float dist = e.o.dist(o);
            if(dist<(e.type==TELEPORT ? 16 : 12)) trypickup(i, d);
        }
    }

    void putitems(packetbuf &p)            // puts items in network stream and also spawns them locally
    {
        putint(p, N_ITEMLIST);
        loopv(ents) if(validitem(ents[i]->type))
        {
            putint(p, i);
            putint(p, ents[i]->type);
        }
        putint(p, -1);
    }

    void resetspawns() { loopv(ents) ents[i]->clearspawned(); }

    void spawnitems(bool force)
    {
        loopv(ents) if(validitem(ents[i]->type))
        {
            ents[i]->setspawned(force || !server::delayspawn(ents[i]->type));
        }
    }

    void setspawn(int i, bool on) { if(ents.inrange(i)) ents[i]->setspawned(on); }

    extentity *newentity() { return new gameentity(); }
    void deleteentity(extentity *e) { delete (gameentity *)e; }

    void clearents()
    {
        while(ents.length()) deleteentity(ents.pop());
    }

    void animatemapmodel(const extentity &e, int &anim, int &basetime)
    {
    }

    void fixentity(extentity &e)
    {
        switch(e.type)
        {
            case FLAG:
                e.attr[4] = e.attr[3];
                e.attr[3] = e.attr[2];
            case TELEDEST:
                e.attr[2] = e.attr[1];
                e.attr[1] = e.attr[0];
                e.attr[0] = (int)player1->yaw;
                break;
        }
    }

    void entradius(extentity &e, bool color)
    {
        switch(e.type)
        {
            case TELEPORT:
                loopv(ents) if(ents[i]->type == TELEDEST && e.attr[0]==ents[i]->attr[1])
                {
                    renderentarrow(e, vec3(ents[i]->o).sub(e.o).normalize(), e.o.dist(ents[i]->o));
                    break;
                }
                break;

            case JUMPPAD:
                renderentarrow(e, vec3((int)(char)e.attr[2]*10.0f, (int)(char)e.attr[1]*10.0f, e.attr[0]*12.5f).normalize(), 4);
                break;

            case FLAG:
            case TELEDEST:
            {
                vec3 dir;
                vecfromyawpitch(e.attr[0], 0, 1, 0, dir);
                renderentarrow(e, dir, 4);
                break;
            }
        }
    }

    bool printent(extentity &e, char *buf, int len)
    {
        return false;
    }

    const char *entnameinfo(entity &e) { return ""; }
    const char *entname(int i)
    {
        static const char * const entnames[MAXENTTYPES] =
        {
            "none?", "light", "mapmodel", "playerstart", "envmap", "particles", "sound", "spotlight", "decal",
            "teleport", "teledest", "jumppad", "flag"
        };
        return i>=0 && size_t(i)<sizeof(entnames)/sizeof(entnames[0]) ? entnames[i] : "";
    }

    const int numattrs(int type)
    {
        static const int num[] =
        { // TODO: Check
            2, //teleport
            2, //teledest
            4, //jumppad
            4, //flag
        };

        type -= ET_GAMESPECIFIC;
        return type >= 0 && size_t(type) < sizeof(num)/sizeof(num[0]) ? num[type] : 0;
    }

    void editent(int i, bool local)
    {
        extentity &e = *ents[i];
        //e.flags = 0;
        if(local) addmsg(N_EDITENT, "rii3ii5", i, (int)(e.o.x*DMF), (int)(e.o.y*DMF), (int)(e.o.z*DMF), e.type, e.attr[0], e.attr[1], e.attr[2], e.attr[3], e.attr[4]);
    }

    float dropheight(entity &e)
    {
        if(e.type==FLAG) return 0.0f;
        return 4.0f;
    }
#endif
}

