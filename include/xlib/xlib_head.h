#ifdef _MSC_VER
	// 4251 -  テンプレートをインスタンス化する前の extern に関する警告を抑制
	// 4786 -  255 文字より長いシンボルを持つコードのデバッグに関する警告を抑制
	// 4231	-  'identifier' がテンプレートの明示的なインスタンス生成に関する警告を抑制
//	#pragma warning( disable: 4251 4786 4231 )
#endif
#ifdef WIN32
	/* Windows用DLL定義 */
#ifdef XLIB_DLL
		#ifdef XLIB_EXPORTS
			#define XLIB_API __declspec(dllexport)
			#define DLL_EXTERN
		#else
			#define XLIB_API __declspec(dllimport)
			#define DLL_EXTERN
		#endif
	#else
		#define XLIB_API
		#define DLL_EXTERN
	#endif
#elif __VMS
	/* VMS用DLL定義 */
	#ifdef XLIB_DLL
		#ifdef __PRAGMA_ENVIRONMENT
		#   pragma __environment save
		#   pragma __environment header_defaults
		#else
		#   pragma __extern_model __save
		#   pragma __extern_prefix __save
		#endif

		#pragma __extern_model __relaxed_refdef
		#pragma __extern_prefix "XLIB_DLL"
	#endif
	#define XLIB_API
	#define DLL_EXTERN
#else
	/* その他DLLなし */
	#define XLIB_API
	#define DLL_EXTERN
#endif
