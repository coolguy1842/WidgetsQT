qt6 = import('qt6')

protoc = find_program('protoc')
gen = generator(protoc,
    output: [ '@BASENAME@.pb.cc', '@BASENAME@.pb.h' ],
    arguments: [ '--proto_path=@CURRENT_SOURCE_DIR@', '--cpp_out=@BUILD_DIR@', '@INPUT@' ]
)

opal_util_sass_embedded_generated = gen.process('embedded_sass.proto')

opal_util_sass_embedded_headers = files(
    'include/Opal/Util/SassEmbedded/sass.hpp'
)

opal_util_sass_embedded_sources = files(
    'src/sass.cpp'
)

opal_util_sass_embedded_compiled_moc = qt6.compile_moc(
    headers: opal_util_sass_embedded_headers,
    preserve_paths: true
)

opal_util_sass_embedded_include = include_directories('include')

opal_util_sass_embedded_lib = static_library(
    'opal_util_sass_embedded',
    sources: [
        opal_util_sass_embedded_sources,
        opal_util_sass_embedded_compiled_moc,
        opal_util_sass_embedded_generated
    ],
    include_directories: opal_util_sass_embedded_include,
    dependencies: [
        dependency('qt6', modules: [ 'Core', 'Network' ]),
        dependency('protobuf')
    ]
)