qt6 = import('qt6')

opal_widgets_headers = files(
    'include/Opal/Widgets/centerbox.hpp',
    'include/Opal/Widgets/window.hpp'
)

opal_widgets_sources = files(
    'src/centerbox.cpp',
    'src/window.cpp'
)

opal_widgets_compiled_moc = qt6.compile_moc(
    headers: opal_widgets_headers,
    preserve_paths: true
)

opal_widgets_include = include_directories('include')

opal_widgets_lib = static_library(
    'opal_widgets',
    sources: [
        opal_widgets_sources,
        opal_widgets_compiled_moc
    ],
    include_directories: opal_widgets_include,
    dependencies: [
        dependency('qt6', modules: [ 'Core', 'Gui', 'Widgets' ]),
        dependency('LayerShellQt', modules: [ 'LayerShellQt::Interface' ])
    ]
)