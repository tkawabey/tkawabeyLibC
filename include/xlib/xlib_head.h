#ifdef _MSC_VER
	// 4251 -  �e���v���[�g���C���X�^���X������O�� extern �Ɋւ���x����}��
	// 4786 -  255 ������蒷���V���{�������R�[�h�̃f�o�b�O�Ɋւ���x����}��
	// 4231	-  'identifier' ���e���v���[�g�̖����I�ȃC���X�^���X�����Ɋւ���x����}��
//	#pragma warning( disable: 4251 4786 4231 )
#endif
#ifdef WIN32
	/* Windows�pDLL��` */
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
	/* VMS�pDLL��` */
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
	/* ���̑�DLL�Ȃ� */
	#define XLIB_API
	#define DLL_EXTERN
#endif
