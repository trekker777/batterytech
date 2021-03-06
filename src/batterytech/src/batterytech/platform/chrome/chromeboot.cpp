/*
 * BatteryTech
 * Copyright (c) 2010 Battery Powered Games LLC.
 *
 * This code is a component of BatteryTech and is subject to the 'BatteryTech
 * End User License Agreement'.  Among other important provisions, this
 * license prohibits the distribution of source code to anyone other than
 * authorized parties.  If you have any questions or would like an additional
 * copy of the license, please contact: support@batterypoweredgames.com
 */

//============================================================================
// Name        : chromeboot.cpp
// Description : Chrome bootstrap
//============================================================================
#ifdef CHROME
#include "../platformgl.h"
#include "../platformgeneral.h"
#include "../../batterytech.h"

#include <stdio.h>
#include <stdlib.h>

#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_errors.h"
#include "ppapi/c/pp_graphics_3d.h"
#include "ppapi/c/pp_module.h"
#include "ppapi/c/pp_var.h"
#include "ppapi/c/ppb.h"
#include "ppapi/c/ppb_core.h"
#include "ppapi/c/ppb_graphics_3d.h"
#include "ppapi/c/ppb_instance.h"
#include "ppapi/c/ppb_messaging.h"
#include "ppapi/c/ppb_opengles2.h"
#include "ppapi/c/ppb_var.h"
#include "ppapi/c/ppp.h"
#include "ppapi/c/ppp_instance.h"
#include "ppapi/c/ppp_messaging.h"
#include "ppapi/c/ppb_url_loader.h"
#include "ppapi/c/ppb_url_request_info.h"

#include "ppapi/c/ppp_graphics_3d.h"
#include "ppapi/lib/gl/gles2/gl2ext_ppapi.h"

PPB_Messaging* ppb_messaging_interface = NULL;
PPB_Var* ppb_var_interface = NULL;
PPB_Instance* ppb_instance_interface = NULL;
PPB_URLRequestInfo* ppb_urlrequestinfo_interface = NULL;
PPB_URLLoader* ppb_urlloader_interface = NULL;
PPB_Core* ppb_core_interface = NULL;
static PPB_Graphics3D* ppb_g3d_interface = NULL;

PP_Instance g_instance;
PP_Resource g_context;

void InitGL(void);


void MainLoop(void* foo, int bar) {
    // TODO - timer
	btUpdate(0.016f);
	btDraw();
    PP_CompletionCallback cc = PP_MakeCompletionCallback(MainLoop, 0);
    ppb_g3d_interface->SwapBuffers(g_context, cc);
}

void InitGL(void)
{
  int32_t attribs[] = {
    PP_GRAPHICS3DATTRIB_ALPHA_SIZE, 8,
    PP_GRAPHICS3DATTRIB_DEPTH_SIZE, 24,
    PP_GRAPHICS3DATTRIB_STENCIL_SIZE, 8,
    PP_GRAPHICS3DATTRIB_SAMPLES, 0,
    PP_GRAPHICS3DATTRIB_SAMPLE_BUFFERS, 0,
    PP_GRAPHICS3DATTRIB_WIDTH, 640,
    PP_GRAPHICS3DATTRIB_HEIGHT, 480,
    PP_GRAPHICS3DATTRIB_NONE
  };

  g_context =  ppb_g3d_interface->Create(g_instance, 0, attribs);
  int32_t success =  ppb_instance_interface->BindGraphics(g_instance, g_context);
  if (success == PP_FALSE)
  {
    glSetCurrentContextPPAPI(0);
    printf("Failed to set context.\n");
    return;
  }
  glSetCurrentContextPPAPI(g_context);

  glViewport(0,0, 640,480);
  glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
  GraphicsConfiguration *g = new GraphicsConfiguration();
  g->supportsFBOs = TRUE;
  g->supportsHWmipmapgen = TRUE;
  g->supportsShaders = TRUE;
  g->supportsUVTransform = TRUE;
  g->supportsVBOs = TRUE;
  g->useShaders = TRUE;
  btInit(g, 640, 480);
}


/**
 * Called when the NaCl module is instantiated on the web page. The identifier
 * of the new instance will be passed in as the first argument (this value is
 * generated by the browser and is an opaque handle).  This is called for each
 * instantiation of the NaCl module, which is each time the <embed> tag for
 * this module is encountered.
 *
 * If this function reports a failure (by returning @a PP_FALSE), the NaCl
 * module will be deleted and DidDestroy will be called.
 * @param[in] instance The identifier of the new instance representing this
 *     NaCl module.
 * @param[in] argc The number of arguments contained in @a argn and @a argv.
 * @param[in] argn An array of argument names.  These argument names are
 *     supplied in the <embed> tag, for example:
 *       <embed id="nacl_module" dimensions="2">
 *     will produce two arguments, one named "id" and one named "dimensions".
 * @param[in] argv An array of argument values.  These are the values of the
 *     arguments listed in the <embed> tag.  In the above example, there will
 *     be two elements in this array, "nacl_module" and "2".  The indices of
 *     these values match the indices of the corresponding names in @a argn.
 * @return @a PP_TRUE on success.
 */
static PP_Bool Instance_DidCreate(PP_Instance instance,
                                  uint32_t argc,
                                  const char* argn[],
                                  const char* argv[]) {
  g_instance = instance;
  // TODO - Init?
  return PP_TRUE;
}


/**
 * Called when the NaCl module is destroyed. This will always be called,
 * even if DidCreate returned failure. This routine should deallocate any data
 * associated with the instance.
 * @param[in] instance The identifier of the instance representing this NaCl
 *     module.
 */
static void Instance_DidDestroy(PP_Instance instance) {
	// TODO - shut down BT
	btRelease();
}

/**
 * Called when the position, the size, or the clip rect of the element in the
 * browser that corresponds to this NaCl module has changed.
 * @param[in] instance The identifier of the instance representing this NaCl
 *     module.
 * @param[in] position The location on the page of this NaCl module. This is
 *     relative to the top left corner of the viewport, which changes as the
 *     page is scrolled.
 * @param[in] clip The visible region of the NaCl module. This is relative to
 *     the top left of the plugin's coordinate system (not the page).  If the
 *     plugin is invisible, @a clip will be (0, 0, 0, 0).
 */
static void Instance_DidChangeView(PP_Instance instance,
                                   PP_Resource view_resource) {
  if (g_context == 0) {
	InitGL();
    MainLoop(NULL, 0);
  }
}

/**
 * Notification that the given NaCl module has gained or lost focus.
 * Having focus means that keyboard events will be sent to the NaCl module
 * represented by @a instance. A NaCl module's default condition is that it
 * will not have focus.
 *
 * Note: clicks on NaCl modules will give focus only if you handle the
 * click event. You signal if you handled it by returning @a true from
 * HandleInputEvent. Otherwise the browser will bubble the event and give
 * focus to the element on the page that actually did end up consuming it.
 * If you're not getting focus, check to make sure you're returning true from
 * the mouse click in HandleInputEvent.
 * @param[in] instance The identifier of the instance representing this NaCl
 *     module.
 * @param[in] has_focus Indicates whether this NaCl module gained or lost
 *     event focus.
 */
static void Instance_DidChangeFocus(PP_Instance instance,
                                    PP_Bool has_focus) {
}

/**
 * Handler that gets called after a full-frame module is instantiated based on
 * registered MIME types.  This function is not called on NaCl modules.  This
 * function is essentially a place-holder for the required function pointer in
 * the PPP_Instance structure.
 * @param[in] instance The identifier of the instance representing this NaCl
 *     module.
 * @param[in] url_loader A PP_Resource an open PPB_URLLoader instance.
 * @return PP_FALSE.
 */
static PP_Bool Instance_HandleDocumentLoad(PP_Instance instance,
                                           PP_Resource url_loader) {
  /* NaCl modules do not need to handle the document load function. */
  return PP_FALSE;
}

/**
 * Entry points for the module.
 * Initialize needed interfaces: PPB_Core, PPB_Messaging and PPB_Var.
 * @param[in] a_module_id module ID
 * @param[in] get_browser pointer to PPB_GetInterface
 * @return PP_OK on success, any other value on failure.
 */
PP_EXPORT int32_t PPP_InitializeModule(PP_Module a_module_id,
                                       PPB_GetInterface get_browser) {
printf("BT - Chromeboot starting...");
  ppb_core_interface = (PPB_Core*)(get_browser(PPB_CORE_INTERFACE));
  ppb_instance_interface = (PPB_Instance*)get_browser(PPB_INSTANCE_INTERFACE);
  ppb_messaging_interface =
      (PPB_Messaging*)(get_browser(PPB_MESSAGING_INTERFACE));
  ppb_var_interface = (PPB_Var*)(get_browser(PPB_VAR_INTERFACE));
  ppb_urlloader_interface =
      (PPB_URLLoader*)(get_browser(PPB_URLLOADER_INTERFACE));
  ppb_urlrequestinfo_interface =
      (PPB_URLRequestInfo*)(get_browser(PPB_URLREQUESTINFO_INTERFACE));
  ppb_g3d_interface = (PPB_Graphics3D*)get_browser(PPB_GRAPHICS_3D_INTERFACE);
  if (!glInitializePPAPI(get_browser))
    return PP_ERROR_FAILED;
  return PP_OK;
}

/**
 * Returns an interface pointer for the interface of the given name, or NULL
 * if the interface is not supported.
 * @param[in] interface_name name of the interface
 * @return pointer to the interface
 */
PP_EXPORT const void* PPP_GetInterface(const char* interface_name) {
  if (strcmp(interface_name, PPP_INSTANCE_INTERFACE) == 0) {
    static PPP_Instance instance_interface = {
      &Instance_DidCreate,
      &Instance_DidDestroy,
      &Instance_DidChangeView,
      &Instance_DidChangeFocus,
      &Instance_HandleDocumentLoad,
    };
    return &instance_interface;
  }
  return NULL;
}

/**
 * Called before the plugin module is unloaded.
 */
PP_EXPORT void PPP_ShutdownModule() {
}






// ---------------------------------------- OS Support Functions ---------------------------------------


// Unsupported GLES1 functions.  Should probably print that GLES1 isn't available on Chrome.
void glEnableClientState (GLenum array) {}
void glDisableClientState (GLenum array) {}
void glShadeModel (GLenum mode) {}
void glVertexPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer) {}
void glColorPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer) {}
void glTexCoordPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer) {}
void glColor4f (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {}
void glPopMatrix (void) {}
void glPushMatrix (void) {}
void glTranslatef (GLfloat x, GLfloat y, GLfloat z) {}
void glRotatef (GLfloat angle, GLfloat x, GLfloat y, GLfloat z) {}
void glScalef (GLfloat x, GLfloat y, GLfloat z) {}
void glTexEnvf (GLenum target, GLenum pname, GLfloat param) {}
void glTexEnvfv (GLenum target, GLenum pname, const GLfloat *params) {}
void glMatrixMode (GLenum mode) {}
void glLoadIdentity (void) {}
void glOrthof (GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar) {}
void glFrustumf (GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar) {}
void glMultMatrixf (const GLfloat *m) {}


// Posix Sockets and netaddr/netinfo

int accept(int socket, struct sockaddr *address, socklen_t *address_len) {
	return 0;
}
int bind(int socket, const struct sockaddr *address, socklen_t address_len) {
	return 0;
}
int connect(int socket, const struct sockaddr *address, socklen_t address_len) {
	return 0;
}
int getpeername(int socket, struct sockaddr *address, socklen_t *address_len) {
	return 0;
}
int getsockname(int socket, struct sockaddr *address, socklen_t *address_len) {
	return 0;
}
int getsockopt(int socket, int level, int option_name, void *option_value,
		socklen_t *option_len) {
	return 0;
}
int listen(int socket, int backlog) {
	return 0;
}
ssize_t recv(int socket, void *buffer, size_t length, int flags) {
	return 0;
}
ssize_t recvfrom(int socket, void *buffer, size_t length, int flags,
		struct sockaddr *address, socklen_t *address_len) {
	return 0;
}
ssize_t recvmsg(int socket, struct msghdr *message, int flags) {
	return 0;
}
ssize_t send(int socket, const void *message, size_t length, int flags) {
	return 0;
}
ssize_t sendmsg(int socket, const struct msghdr *message, int flags) {
	return 0;
}
ssize_t sendto(int socket, const void *message, size_t length, int flags,
		const struct sockaddr *dest_addr, socklen_t dest_len) {
	return 0;
}
int setsockopt(int socket, int level, int option_name,
		const void *option_value, socklen_t option_len) {
	return 0;
}
int shutdown(int socket, int how) {
	return 0;
}
int socket(int domain, int type, int protocol) {
	return 0;
}
int socketpair(int domain, int type, int protocol, int socket_vector[2]) {
	return 0;
}

int getaddrinfo(const char *hostname, const char *service,
		const struct addrinfo *hints, struct addrinfo **res) {
	return 0;
}
int getnameinfo(const struct sockaddr *sa, socklen_t salen, char *host,
		size_t hostlen, char *serv, size_t servlen, int flags) {
	return 0;
}
void freeaddrinfo(struct addrinfo *ai) {
}
const char* inet_ntop(int af, const void *src, char *dst, socklen_t size) {
	return "";
}
int inet_pton(int af, const char *src, void *dst) {
	return 0;
}

#endif /* chrome */
