/*
 * Initial main.c file generated by Glade. Edit as required.
 * Glade will not overwrite this file.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <gtk/gtkgl.h>
//#include <gdk/gdkglglext.h>
#include <glade/glade.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include "support.h"

#include "callbacks.h"

#ifdef G_OS_WIN32
char *package_prefix;
char *package_datadir;
#endif


/* globals for now */
GtkWidget* main_window;
GtkWidget* gsc_quit_dialog;

/* GdkGL_GL_ARB_vertex_program* gdk_glext_vp; */

/* static void init_gl_ext(void) */
/* { */
/*   gdk_glext_vp =gdk_gl_get_GL_ARB_vertex_program(); */
/*   if(NULL == gdk_glext_vp ) { */
/*     g_print("ERROR: Failed to init GL_ARB_vertex_program ext !/n"); */
/*     exit(-1); */
/*   } */
  
/*   g_print("DEBUG: GL_ARB_vertex_program ext init success.../n"); */
/* } */


int
main(int argc, char *argv[] )
{

  GladeXML* xml;
  GdkGLConfig* glconfig;
  GtkWidget* drawing_area;

  gboolean dummy;

  struct shader_txt_buffers_t shader_buffers;

#ifdef G_OS_WIN32
  gchar *temp;

  package_prefix = g_win32_get_package_installation_directory (NULL, NULL);
  package_datadir = g_strdup_printf ("%s%s", package_prefix, "/share");
#endif

#ifdef ENABLE_NLS
#ifdef G_OS_WIN32
  temp = g_strdup_printf ("%s%s", package_prefix, "/lib/locale");
  bindtextdomain (GETTEXT_PACKAGE, temp);
  g_free (temp);
#else
  bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
#endif
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);
#endif

  //gtk_set_locale ();

  gtk_init(&argc, &argv );
  gtk_gl_init(&argc, &argv );

  /* Try double-buffered visual */
  glconfig = gdk_gl_config_new_by_mode (GDK_GL_MODE_RGB | GDK_GL_MODE_DEPTH | GDK_GL_MODE_DOUBLE );
  if(glconfig == NULL) {
    g_print ("*** Cannot find the double-buffered visual.\n");
    g_print ("*** Trying single-buffered visual.\n");

    /* Try single-buffered visual */
    glconfig = gdk_gl_config_new_by_mode(GDK_GL_MODE_RGB | GDK_GL_MODE_DEPTH );
    if(glconfig == NULL) {
      g_print ("*** No appropriate OpenGL-capable visual found.\n");
      exit (1);
    }
  }


#ifdef G_OS_WIN32
  temp = g_strdup_printf ("%s/%s%s", package_datadir, PACKAGE, "/pixmaps");
  add_pixmap_directory (temp);
  g_free (temp);
#else
  add_pixmap_directory (PACKAGE_DATA_DIR "/" PACKAGE "/pixmaps");
#endif


  xml =glade_xml_new ("gnu_shader_composer.glade", NULL, NULL );

  //autoconnect does NOT work with data !
  //glade_xml_signal_autoconnect(xml );

  glade_xml_signal_connect(xml, 
			   "on_drawing_area_realize", 
			   G_CALLBACK(on_drawing_area_realize ) );

  glade_xml_signal_connect(xml, 
			   "on_drawing_area_configure_event", 
			   G_CALLBACK(on_drawing_area_configure_event ) );

  glade_xml_signal_connect(xml, 
			   "on_drawing_area_expose_event", 
			   G_CALLBACK(on_drawing_area_expose_event ) );

//
  glade_xml_signal_connect(xml, 
			   "button_press_event", 
			   G_CALLBACK(button_press_event ) );
  
  glade_xml_signal_connect(xml, 
			   "button_release_event", 
			   G_CALLBACK(button_release_event ) );

  glade_xml_signal_connect(xml, 
			   "motion_notify_event", 
			   G_CALLBACK(motion_notify_event ) );



  glade_xml_signal_connect(xml, 
			   "on_main_window_delete_event", 
			   G_CALLBACK(on_main_window_delete_event) );

  glade_xml_signal_connect_data(xml, 
				"exit_dlg_button_no_clicked", 
				G_CALLBACK(exit_dlg_button_no_clicked),
				(gpointer)"Secret Squirell" );//test data

  glade_xml_signal_connect(xml, 
			   "exit_dlg_button_quit_clicked", 
			   G_CALLBACK(exit_dlg_button_quit_clicked) );

  glade_xml_signal_connect(xml, 
			   "on_hpaned1_check_resize", 
			   G_CALLBACK(on_hpaned1_check_resize ) );

  glade_xml_signal_connect(xml, 
			   "on_hpaned1_accept_position", 
			   G_CALLBACK(on_hpaned1_accept_position ) );

  glade_xml_signal_connect(xml, 
			   "on_hpaned1_state_changed", 
			   G_CALLBACK(on_hpaned1_state_changed ) );

  glade_xml_signal_connect(xml, 
			   "on_hpaned1_move_handle", 
			   G_CALLBACK(on_hpaned1_move_handle ) );

  /* main menu, file menu, quit item */
  glade_xml_signal_connect(xml, 
			   "on_main_menu_item_file_quit", 
			   G_CALLBACK(on_main_menu_item_file_quit ) );

  
  glade_xml_signal_connect_data(xml, 
				"on_toolbutton_compile_execute_shader_clicked", 
				G_CALLBACK(on_toolbutton_compile_execute_shader_clicked ),
				&shader_buffers );  
 
  main_window = glade_xml_get_widget(xml, "main_window" );

  //pre-create the dialogs
  gsc_quit_dialog =glade_xml_get_widget(xml, "exit_confirmation_dialog" );

  //gtk_widget_set_parent(gsc_quit_dialog, main_window );
  gtk_widget_set_parent_window(gsc_quit_dialog, //widget
 			       main_window );   //parent

  gtk_container_set_reallocate_redraws (GTK_CONTAINER(main_window), TRUE );

  printf("drawing_area: %u\n", drawing_area );
  drawing_area = glade_xml_get_widget (xml, "drawing_area" );
  printf("drawing_area: %u\n", drawing_area );

  /* Add OpenGL-capability to drawingarea1. */
  dummy =gtk_widget_set_gl_capability(drawing_area, 
				      glconfig, 
				      NULL, 
				      TRUE, 
				      GDK_GL_RGBA_TYPE );

  printf("dummy: %s\n", dummy?"TRUE":"FALSE" );

  shader_buffers.vp_buffer =gtk_text_view_get_buffer(glade_xml_get_widget(xml, "vp_textview" ) );
  shader_buffers.fp_buffer =gtk_text_view_get_buffer(glade_xml_get_widget(xml, "fp_textview" ) );

  glade_xml_signal_connect_data(xml, 
				"on_toolbutton_compile_execute_shader_clicked", 
				G_CALLBACK(on_toolbutton_compile_execute_shader_clicked ),
				&shader_buffers );

  gtk_widget_show(main_window);

  /* the GL extension must be loaded AFTER the gl widget was shown */
  //init_gl_ext();

  /*
  g_signal_connect( (gpointer)main_window, 
		    "destroy",
		    G_CALLBACK(gtk_main_quit),
                     NULL);

  */

  gtk_main ();

#ifdef G_OS_WIN32
  g_free (package_prefix);
  g_free (package_datadir);
#endif
  return 0;
}
#ifdef _MSC_VER
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
	return main (__argc, __argv);
}
#endif

