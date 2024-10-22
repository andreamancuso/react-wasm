{
    "targets": [
        {
            "target_name": "node-imgui",
            "cflags!": [ "-fno-exceptions" ],
            "cflags_cc!": [ "-fno-exceptions" ],
            "sources": [
                "./hello.cc",

                "<!@(node -p \"require('fs').readdirSync('../deps/yoga/yoga').filter(f => f.endsWith('.cpp')).map(f => '../deps/yoga/yoga/'+f).join(' ')\")",
                "<!@(node -p \"require('fs').readdirSync('../deps/yoga/yoga/algorithm').filter(f => f.endsWith('.cpp')).map(f => '../deps/yoga/yoga/algorithm/'+f).join(' ')\")",
                "<!@(node -p \"require('fs').readdirSync('../deps/yoga/yoga/config').filter(f => f.endsWith('.cpp')).map(f => '../deps/yoga/yoga/config/'+f).join(' ')\")",
                "<!@(node -p \"require('fs').readdirSync('../deps/yoga/yoga/debug').filter(f => f.endsWith('.cpp')).map(f => '../deps/yoga/yoga/debug/'+f).join(' ')\")",
                "<!@(node -p \"require('fs').readdirSync('../deps/yoga/yoga/enums').filter(f => f.endsWith('.cpp')).map(f => '../deps/yoga/yoga/enums/'+f).join(' ')\")",
                "<!@(node -p \"require('fs').readdirSync('../deps/yoga/yoga/event').filter(f => f.endsWith('.cpp')).map(f => '../deps/yoga/yoga/event/'+f).join(' ')\")",
                "<!@(node -p \"require('fs').readdirSync('../deps/yoga/yoga/node').filter(f => f.endsWith('.cpp')).map(f => '../deps/yoga/yoga/node/'+f).join(' ')\")",
                "<!@(node -p \"require('fs').readdirSync('../deps/yoga/yoga/numeric').filter(f => f.endsWith('.cpp')).map(f => '../deps/yoga/yoga/numeric/'+f).join(' ')\")",
                "<!@(node -p \"require('fs').readdirSync('../deps/yoga/yoga/style').filter(f => f.endsWith('.cpp')).map(f => '../deps/yoga/yoga/style/'+f).join(' ')\")",

                "./deps/imgui/imgui.cpp",
                "./deps/imgui/imgui_draw.cpp",
                "./deps/imgui/imgui_widgets.cpp",
                "./deps/imgui/imgui_tables.cpp",
                "./deps/imgui/backends/imgui_impl_glfw.cpp",
                "./deps/imgui/backends/imgui_impl_opengl3.cpp",

                "../deps/implot/implot.cpp",
                "../deps/implot/implot_items.cpp",

                "../app/src/shared.cpp",
                "../app/src/color_helpers.cpp",
                "../app/src/yoga_helpers.cpp",
                "../app/src/imgui_helpers.cpp",

                "../app/src/element/layout_node.cpp",
                "../app/src/element/element.cpp",

                "../app/src/widget/widget.cpp",
                "../app/src/widget/styled_widget.cpp",

                "../app/src/widget/button.cpp",
                "../app/src/widget/checkbox.cpp",
                "../app/src/widget/child.cpp",
                "../app/src/widget/clipped_multi_line_text_renderer.cpp",
                "../app/src/widget/collapsing_header.cpp",
                "../app/src/widget/combo.cpp",
                "../app/src/widget/group.cpp",
                "../app/src/widget/input_text.cpp",
                "../app/src/widget/item_tooltip.cpp",
                "../app/src/widget/multi_slider.cpp",
                "../app/src/widget/plot_candlestick.cpp",
                "../app/src/widget/plot_line.cpp",
                "../app/src/widget/separator.cpp",
                "../app/src/widget/separator_text.cpp",
                "../app/src/widget/slider.cpp",
                "../app/src/widget/table.cpp",
                "../app/src/widget/tabs.cpp",
                "../app/src/widget/text.cpp",
                "../app/src/widget/text_wrap.cpp",
                "../app/src/widget/tree_node.cpp",
                "../app/src/widget/window.cpp",

                "../app/src/reactimgui.cpp",
                "../app/src/imgui_renderer.cpp",
                "../app/src/implot_renderer.cpp",
            ],
            "include_dirs": [
                "<!@(node -p \"require('node-addon-api').include\")",
                "./deps/imgui",
                "./deps/imgui/backends",
                "../deps/implot",
                "../deps/yoga",
                "./deps/glfw/include"
            ],
            'cflags_cc': ['-std=c++23', '-fno-exceptions'],
            'cflags': ['-std=c++23', '-fno-exceptions'],
            'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
            'conditions': [
                ['OS=="win"', {
                    'libraries': ['../glfw3dll.lib', '../opengl32.lib'],
                    'defines': ['WIN32_LEAN_AND_MEAN', 'VC_EXTRALEAN', '_WIN32', '_HAS_EXCEPTIONS=0'],
                    'msvs_settings' : {
                        'VCCLCompilerTool' : {
                            'AdditionalOptions' : [
                                '/O2','/Oy','/GL','/GF','/Gm-',
                                '/EHa-s-c-','/MT','/GS','/Gy','/GR-','/Gd',
                            ]
                        },
                        'VCLinkerTool' : {
                            'AdditionalOptions' : ['/DEBUG:NONE', '/LTCG', '/OPT:NOREF'],
                        },
                    },
                }],
            ],
        }
    ]
}