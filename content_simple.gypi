# Copyright (c) 2012 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'variables': {
    'content_simple_product_name': 'Content Simple',
    # The "19" is so that sites that sniff for version think that this is
    # something reasonably current; the "77.34.5" is a hint that this isn't a
    # standard Chrome.
    'content_simple_version': '19.77.34.5',
    'conditions': [
    ],
  },
  'targets': [
    {
      'target_name': 'content_simple_lib',
      'type': 'static_library',
      'defines': ['CONTENT_SIMPLE_VERSION="<(content_simple_version)"'],
      'variables': {
        'chromium_code': 1,
      },
      'dependencies': [
        'content.gyp:content_app_both',
        'content.gyp:content_browser',
        'content.gyp:content_common',
        'content.gyp:content_gpu',
        'content.gyp:content_plugin',
        'content.gyp:content_ppapi_plugin',
        'content.gyp:content_renderer',
        'content.gyp:content_utility',
        'content.gyp:content_worker',
        'content_resources.gyp:content_resources',
        'content_simple_resources',
        '../base/base.gyp:base',
        '../base/base.gyp:base_static',
        '../base/third_party/dynamic_annotations/dynamic_annotations.gyp:dynamic_annotations',
        '../cc/cc.gyp:cc',
        '../components/components.gyp:breakpad_component',
        '../gin/gin.gyp:gin',
        '../gpu/gpu.gyp:gpu',
        '../ipc/ipc.gyp:ipc',
        '../media/media.gyp:media',
        '../net/net.gyp:net',
        '../net/net.gyp:net_resources',
        '../skia/skia.gyp:skia',
        '../third_party/WebKit/public/blink.gyp:blink',
        '../third_party/WebKit/public/blink.gyp:blink_test_support',
        '../ui/base/ui_base.gyp:ui_base',
        '../ui/events/events.gyp:events_base',
        '../ui/gfx/gfx.gyp:gfx',
        '../ui/gfx/gfx.gyp:gfx_geometry',
        '../ui/gl/gl.gyp:gl',
        '../url/url.gyp:url_lib',
        '../v8/tools/gyp/v8.gyp:v8',
        '../webkit/common/webkit_common.gyp:webkit_common',
        '../webkit/storage_browser.gyp:webkit_storage_browser',
        '../webkit/webkit_resources.gyp:webkit_resources',
      ],
      'include_dirs': [
        '..',
      ],
      'sources': [
	  'simple/app/simple_main_delegate.cc',
	  'simple/app/simple_main_delegate.h',
	  'simple/browser/simple_content_browser_client.cc',
	  'simple/browser/simple_content_browser_client.h',
	  'simple/browser/simple_browser_main_parts.cc',
	  'simple/browser/simple_browser_main_parts.h',
	  'simple/browser/simple_browser_context.cc',
	  'simple/browser/simple_browser_context.h',
	  'simple/browser/simple_net_log.cc',
	  'simple/browser/simple_net_log.h',
	  'simple/browser/simple_url_request_context_getter.cc',
	  'simple/browser/simple_url_request_context_getter.h',
	  'simple/browser/simple_web_contents_delegate.cc',
	  'simple/browser/simple_web_contents_delegate.h',
	  'simple/browser/simple_web_contents_view_delegate.cc',
	  'simple/browser/simple_web_contents_view_delegate.h',
      ],
      'msvs_settings': {
        'VCLinkerTool': {
          'SubSystem': '2',  # Set /SUBSYSTEM:WINDOWS
        },
      },
      'conditions': [
        ['OS=="win" and win_use_allocator_shim==1', {
          'dependencies': [
            '../base/allocator/allocator.gyp:allocator',
          ],
        }],
        ['OS=="win"', {
          'resource_include_dirs': [
            '<(SHARED_INTERMEDIATE_DIR)/webkit',
          ],
          'dependencies': [
            '<(DEPTH)/webkit/webkit_resources.gyp:webkit_strings',
          ],
          'configurations': {
            'Debug_Base': {
              'msvs_settings': {
                'VCLinkerTool': {
                  'LinkIncremental': '<(msvs_large_module_debug_link_mode)',
                },
              },
            },
          },
          # TODO(jschuh): crbug.com/167187 fix size_t to int truncations.
          'msvs_disabled_warnings': [ 4267, ],
        }, {  # OS!="win"
          'sources/': [
            ['exclude', 'Win\\.cpp$'],
          ],
        }],  # OS=="win"
        ['use_aura==1', {
          'dependencies': [
            '../ui/aura/aura.gyp:aura',
            '../ui/aura/aura.gyp:aura_test_support',
            '../ui/events/events.gyp:events',
            '../ui/strings/ui_strings.gyp:ui_strings',
            '../ui/wm/wm.gyp:wm',
          ],
          'conditions': [
            ['toolkit_views==1', {
              'dependencies': [
                '../ui/resources/ui_resources.gyp:ui_resources',
                '../ui/views/controls/webview/webview.gyp:webview',
                '../ui/views/views.gyp:views',
                '../ui/views/views.gyp:views_test_support',
              ],
              'sources/': [
                ['exclude', 'simple/browser/simple_aura.cc'],
              ],
            }, {
              'sources/': [
                ['exclude', 'simple/browser/simple_views.cc'],
              ],
            }],
          ],
        }, {
          'sources/': [
            ['exclude', 'simple/browser/simple_aura.cc'],
            ['exclude', 'simple/browser/simple_views.cc'],
          ],
        }],  # use_aura==1
        ['enable_plugins==0', {
          'sources/': [
            ['exclude', 'simple/browser/simple_plugin_service_filter.cc'],
            ['exclude', 'simple/browser/simple_plugin_service_filter.h'],
          ],
        }]
      ],
    },
    {
      'target_name': 'content_simple_resources',
      'type': 'none',
      'variables': {
        'grit_out_dir': '<(SHARED_INTERMEDIATE_DIR)/content',
      },
      'actions': [
        {
          'action_name': 'generate_content_simple_resources',
          'variables': {
            'grit_grd_file': 'simple/simple_resources.grd',
          },
          'includes': [ '../build/grit_action.gypi' ],
        },
      ],
      'includes': [ '../build/grit_target.gypi' ],
      'copies': [
        {
          'destination': '<(PRODUCT_DIR)',
          'files': [
            '<(SHARED_INTERMEDIATE_DIR)/content/simple_resources.pak'
          ],
        },
      ],
      'conditions': [
        ['OS=="win"', {
          'copies': [{
            'destination': '<(PRODUCT_DIR)',
            'files': ['simple/renderer/test_runner/resources/fonts/AHEM____.TTF'],
          }],
        }],
      ],
    },
    {
      # We build a minimal set of resources so WebKit in content_shell has
      # access to necessary resources.
      'target_name': 'content_simple_pak',
      'type': 'none',
      'dependencies': [
        'content_resources.gyp:content_resources',
        'content_simple_resources',
        '<(DEPTH)/net/net.gyp:net_resources',
        '<(DEPTH)/ui/resources/ui_resources.gyp:ui_resources',
        '<(DEPTH)/ui/strings/ui_strings.gyp:ui_strings',
        '<(DEPTH)/webkit/webkit_resources.gyp:webkit_resources',
        '<(DEPTH)/webkit/webkit_resources.gyp:webkit_strings',
      ],
      'conditions': [
        ['OS!="android" and OS!="ios"', {
          'dependencies': [
            'browser/tracing/tracing_resources.gyp:tracing_resources',
          ],
        }],
        ['OS!="android"', {
          'dependencies': [
            'browser/devtools/devtools_resources.gyp:devtools_resources',
          ],
        }],
      ],
      'actions': [
        {
          'action_name': 'repack_content_simple_pack',
          'variables': {
            'pak_inputs': [
              '<(SHARED_INTERMEDIATE_DIR)/content/content_resources.pak',
              '<(SHARED_INTERMEDIATE_DIR)/content/browser/tracing/tracing_resources.pak',
              '<(SHARED_INTERMEDIATE_DIR)/content/simple_resources.pak',
              '<(SHARED_INTERMEDIATE_DIR)/net/net_resources.pak',
              '<(SHARED_INTERMEDIATE_DIR)/ui/app_locale_settings/app_locale_settings_en-US.pak',
              '<(SHARED_INTERMEDIATE_DIR)/ui/ui_resources/ui_resources_100_percent.pak',
              '<(SHARED_INTERMEDIATE_DIR)/ui/ui_resources/webui_resources.pak',
              '<(SHARED_INTERMEDIATE_DIR)/ui/ui_strings/ui_strings_en-US.pak',
              '<(SHARED_INTERMEDIATE_DIR)/webkit/blink_resources.pak',
              '<(SHARED_INTERMEDIATE_DIR)/webkit/webkit_resources_100_percent.pak',
              '<(SHARED_INTERMEDIATE_DIR)/webkit/webkit_strings_en-US.pak',
            ],
            'conditions': [
              ['OS!="android"', {
                'pak_inputs': ['<(SHARED_INTERMEDIATE_DIR)/webkit/devtools_resources.pak',],
                'pak_output': '<(PRODUCT_DIR)/content_simple.pak',
              }, {
                'pak_output': '<(PRODUCT_DIR)/content_simple/assets/content_simple.pak',
              }],
            ],
          },
          'includes': [ '../build/repack_action.gypi' ],
        },
      ],
    },
    {
      'target_name': 'content_simple',
      'type': 'executable',
      'mac_bundle': 1,
      'variables': {
        'chromium_code': 1,
      },
      'dependencies': [
        'content_simple_lib',
        'content_simple_pak',
        '../third_party/mesa/mesa.gyp:osmesa',
        '../tools/imagediff/image_diff.gyp:image_diff',
      ],
      'include_dirs': [
        '..',
      ],
      'sources': [
        'app/startup_helper_win.cc',
        'simple/app/simple_main.cc',
      ],
      'msvs_settings': {
        'VCLinkerTool': {
          'SubSystem': '2',  # Set /SUBSYSTEM:WINDOWS
        },
        'VCManifestTool': {
          'AdditionalManifestFiles': [
            'simple/app/simple.exe.manifest',
          ],
        },
      },
      'conditions': [
        ['OS=="win" and win_use_allocator_shim==1', {
          'dependencies': [
            '../base/allocator/allocator.gyp:allocator',
          ],
        }],
        ['OS=="win"', {
          'sources': [
            'simple/app/simple.rc',
          ],
          'configurations': {
            'Debug_Base': {
              'msvs_settings': {
                'VCLinkerTool': {
                  'LinkIncremental': '<(msvs_large_module_debug_link_mode)',
                },
              },
            },
          },
        }],  # OS=="win"
        ['OS == "win"', {
          'dependencies': [
            '../sandbox/sandbox.gyp:sandbox',
          ],
        }],  # OS=="win"
      ],
    },
	{
      'target_name': 'content_simple_builder',
      'type': 'none',
      'dependencies': [
        'content_simple',
      ],
    },
    
  ],
  'conditions': [
    ['OS=="mac" or OS=="win"', {
      'targets': [
      ],
    }],  # OS=="mac" or OS=="win"
    ['OS=="win"', {
      'targets': [
      ],
    }],  # OS=="win"
  ]
}
