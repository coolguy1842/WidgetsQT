config_headers = files(
    'include/widgets/bar.hpp',
)

config_sources = files(
    'src/widgets/bar.cpp',

    'src/main.cpp',
)

config_compiled_moc = qt6.compile_moc(
    headers: config_headers,
    sources: config_sources,
    preserve_paths: true
)

config_include = include_directories('include')

executable(
    'widgets',
    sources: [
        config_sources,
        config_compiled_moc
    ],
    include_directories: [
        core_include,
        config_include
    ],
    dependencies: [
        dependency('qt6', modules: [ 'Core', 'Gui', 'Widgets' ])
    ],
    link_with : [
        core_lib
    ]
)
