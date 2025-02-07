#include "include/mpris_handler.h"
#include "../dbus/mpris.h"
#include "glib.h"
#include "include/mpris_signal_handler.h"
#include <cstddef>


comet::mpris_handler::mpris_handler(comet::mpris_signal_handler* signal_handler){

    this->signal_handler = signal_handler;

    loop = g_main_loop_new(NULL, FALSE);


    owned_id = g_bus_own_name(G_BUS_TYPE_SESSION,
        server_bus_name.c_str(),
        G_BUS_NAME_OWNER_FLAGS_REPLACE,
        on_bus_acquired,
        on_name_acquired,
        NULL, 
        NULL,
        NULL);


}

void comet::mpris_handler::handle_method_call (GDBusConnection       *connection,
                    const gchar           *sender,
                    const gchar           *object_path,
                    const gchar           *interface_name,
                    const gchar           *method_name,
                    GVariant              *parameters,
                    GDBusMethodInvocation *invocation,
                    gpointer               user_data) 
                {
                    if (g_strcmp0 (method_name, "Play") == 0)
                    {
                        // comet_player->mpris_function_play();
                        signal_handler->push_signal(comet::mpris_signal_handler::signal::PLAY);
                        comet_media_player2_player_complete_play(skeleton, invocation);

                    }

                    if (g_strcmp0 (method_name, "PlayPause") == 0)
                    {
                        signal_handler->push_signal(comet::mpris_signal_handler::signal::PLAY_PAUSE);
                        // comet_player->mpris_function_playpause();
                        comet_media_player2_player_complete_play_pause(skeleton, invocation);
                    }


                    //these two do not follow the specification strictly but for now i do not mind.
                    if (g_strcmp0 (method_name, "Next") == 0)
                    {
                        // comet_player->play_next();
                        signal_handler->push_signal(comet::mpris_signal_handler::signal::PLAY_NEXT);

                        comet_media_player2_player_complete_next(skeleton, invocation);
                    }
                     if (g_strcmp0 (method_name, "Previous") == 0)
                    {
                        signal_handler->push_signal(comet::mpris_signal_handler::signal::PREVIOUS);

                        // comet_player->play_next(false);
                        comet_media_player2_player_complete_previous(skeleton, invocation);
                    }


                }

void comet::mpris_handler::on_bus_acquired(GDBusConnection *connection, const gchar *name, gpointer user_data){

    guint registration_id;

    registration_id = g_dbus_connection_register_object (connection,
                                                        "/org/mpris/MediaPlayer2",
                                                        comet_media_player2_player_interface_info(),
                                                        &interface_vtable,
                                                        NULL,  /* user_data */
                                                        NULL,  /* user_data_free_func */
                                                        NULL); /* GError** */
    g_assert (registration_id > 0);

}


GVariant* comet::mpris_handler::handle_get_property(GDBusConnection  *connection,
                     const gchar      *sender,
                     const gchar      *object_path,
                     const gchar      *interface_name,
                     const gchar      *property_name,
                     GError          **error,
                     gpointer          user_data)
                     
                    {
                        GVariant *ret;
                        ret = NULL;

                        if (g_strcmp0 (property_name, "Volume") == 0)
                        {
                        //Temporary value while I handle encapsulation
                        ret = g_variant_new_double (0.0f);
                        }

                        return ret;

                    }

gboolean comet::mpris_handler::handle_set_property (GDBusConnection  *connection,
                     const gchar      *sender,
                     const gchar      *object_path,
                     const gchar      *interface_name,
                     const gchar      *property_name,
                     GVariant         *value,
                     GError          **error,
                     gpointer          user_data)
                    {   
                        GError* error_local {NULL};
                        
                        //This does not work!! I have no idea why.
                        if (g_strcmp0 (property_name, "Volume") == 0)
                        {
                            
                            auto volume {g_variant_get_double(value)};

                            //Temporary value while I handle encapsulation
                            // comet_player->set_volume(volume);

                            g_dbus_connection_emit_signal (connection,
                                         NULL,
                                         object_path,
                                         "org.freedesktop.DBus.Properties",
                                         "PropertiesChanged",
                                         value,
                                         &error_local);

                            g_assert_no_error(error_local);

                        }

                        return error_local == NULL;
                    }


                     

void comet::mpris_handler::on_name_acquired(GDBusConnection *connection, const gchar *name, gpointer user_data){


    skeleton = comet_media_player2_player_skeleton_new();

    // g_signal_connect(skeleton, "handle_play", G_CALLBACK(on_handle_play), NULL);

    g_dbus_interface_skeleton_export(
        G_DBUS_INTERFACE_SKELETON(skeleton), 
        connection,
        "/org/mpris/MediaPlayer2",
        NULL);
}

void comet::mpris_handler::emit_property_changed(){

}

void comet::mpris_handler::run(){
    g_main_loop_run(loop);
}

void comet::mpris_handler::stop(){
    g_main_loop_quit(loop);
    g_bus_unown_name(owned_id);
}


// gboolean comet::mpris_handler::on_handle_play(cometMediaPlayer2Player * skeleton, GDBusMethodInvocation* invocation) {
//     // comet_player->mpris_function_play();
//     // comet_media_player2_player_complete_play(skeleton, invocation);

//     // gchar* response;
//     // response = g_strdup_printf ("Hello world!!.");
//     // g_print("%s\n", response);
// 	// g_free (response);
//     return true;;   
// }
