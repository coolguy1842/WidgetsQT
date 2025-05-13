qt6 = import('qt6')

core_headers = files(
    'include/widgets/window.hpp',

    'include/application.hpp'
)

core_sources = files(
    'src/widgets/window.cpp',
    
    'src/application.cpp'
)

core_compiled_moc = qt6.compile_moc(
    headers: core_headers,
    sources: core_sources,
    preserve_paths: true
)

core_include = include_directories('include')

common_args = [
    '-O0',
    '-ggdb3'
]

core_lib = static_library(
    'core',
    sources: [
        core_sources,
        core_compiled_moc
    ],
    include_directories: core_include,
    dependencies: [
        dependency('qt6', modules: [ 'Core', 'Gui', 'Widgets' ]),
        dependency('LayerShellQt', modules: [ 'LayerShellQt::Interface' ])
    ],
    c_args: common_args,
    cpp_args: common_args,
    link_args: common_args
)