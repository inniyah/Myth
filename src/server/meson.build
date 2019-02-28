client_src = [
    '../shared/crypto.cc',
    '../shared/stream.cc',
    '../shared/tools.cc',
    '../engine/command.cc',
    '../engine/server.cc',
    '../engine/worldio.cc',
    '../game/entities.cc',
    '../game/server.cc'
]

threads_dep = dependency('threads')
libenet_dep = dependency('libenet')
zlib_dep = dependency('zlib')

executable('native_server',
    client_src,
    dependencies: [
        threads_dep, libenet_dep, zlib_dep
    ],
    include_directories: octacore_includes,
    cpp_args: ['-DSTANDALONE'],
    install: true,
    install_dir: join_paths(meson.source_root(), 'bin_unix')
)
