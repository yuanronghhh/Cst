#include <Framework/Media/FrGlRender.h>
#include <Framework/Media/FrVideoFrame.h>
#include <Framework/Media/FrIMediaRender.h>


static void i_media_render_imp(FrIMediaRenderInterface *iface);

SYS_DEFINE_WITH_CODE(FrGlRender, fr_gl_render, SYS_TYPE_OBJECT,
    SYS_IMPLEMENT_INTERFACE(FR_TYPE_I_MEDIA_RENDER, i_media_render_imp));

static GLuint gl_generate_texture(void) {
  // Generate texture
  GLuint tex_id;
  glGenTextures(1, &tex_id);
  glBindTexture(GL_TEXTURE_2D, tex_id);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  return tex_id;
}

static void clear_frame_buffer(FrGlRender *self) {
  GLFWwindow *window = self->window;

  SysInt width = 0, height = 0;

  glfwGetFramebufferSize(window, &width, &height);
  self->width = width;
  self->height = height;

  glViewport(0, 0, width, height);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, width, height, 0, -1, 1);
  glMatrixMode(GL_MODELVIEW);
}

static void i_media_render_video(FrIMediaRender *o, FrVideoFrame *frame, FrRegion *region) {
  FrGlRender *self = FR_GL_RENDER(o);
  GLuint tex_id = self->tex_id;

  clear_frame_buffer(self);
  glBindTexture(GL_TEXTURE_2D, tex_id);
  glTexImage2D(GL_TEXTURE_2D,
      0, 
      GL_RGB, 
      self->width, 
      self->height, 
      0, 
      GL_RGBA, 
      GL_UNSIGNED_BYTE, frame->parent.ctx->data);

  // Render whatever you want
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, tex_id);
  glBegin(GL_QUADS);
  glTexCoord2d(0, 0); glVertex2i(200, 200);
  glTexCoord2d(1, 0); glVertex2i(200 + self->width, 200);
  glTexCoord2d(1, 1); glVertex2i(200 + self->width, 200 + self->height);
  glTexCoord2d(0, 1); glVertex2i(200, 200 + self->height);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

static void i_media_render_audio(FrIMediaRender *o, FrAudioFrame *frame) {
  // FrGlRender *self = FR_GL_RENDER(o);
}

static void i_media_render_imp(FrIMediaRenderInterface *iface) {
  iface->render_video = i_media_render_video;
  iface->render_audio = i_media_render_audio;
}

void init_window(GLFWwindow *window) {
  glfwMakeContextCurrent(window);
  gladLoadGL();
  glfwSwapInterval(1);
}

/* object api */
static void fr_gl_render_construct(FrGlRender *self, GLFWwindow *window) {
  init_window(window);

  self->window = window;
  self->tex_id = gl_generate_texture();
}

FrGlRender* fr_gl_render_new(void) {
  return sys_object_new(FR_TYPE_GL_RENDER, NULL);
}

FrGlRender *fr_gl_render_new_I(GLFWwindow *window) {
  FrGlRender *o = fr_gl_render_new();

  fr_gl_render_construct(o, window);

  return o;
}

static void fr_gl_render_dispose(SysObject* o) {
  FrGlRender *self = FR_GL_RENDER(o);

  UNUSED(self);

  SYS_OBJECT_CLASS(fr_gl_render_parent_class)->dispose(o);
}

static void fr_gl_render_class_init(FrGlRenderClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_gl_render_dispose;
}

void fr_gl_render_init(FrGlRender* self) {
}
