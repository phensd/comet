#pragma once
#include <glib.h>
#include "../dbus/mpris.h"
#include "gio/gio.h"
#include "mpris_signal_handler.h"

namespace comet {
    class mpris_handler{

    GMainLoop* loop;
    inline static mpris_signal_handler* signal_handler;

    inline static cometMediaPlayer2Player* skeleton;


    // static gboolean on_handle_play(cometMediaPlayer2Player * skeleton, GDBusMethodInvocation* invocation);


    static void handle_method_call (GDBusConnection       *connection,
                    const gchar           *sender,
                    const gchar           *object_path,
                    const gchar           *interface_name,
                    const gchar           *method_name,
                    GVariant              *parameters,
                    GDBusMethodInvocation *invocation,
                    gpointer               user_data);


    static GVariant* handle_get_property(GDBusConnection  *connection,
                     const gchar      *sender,
                     const gchar      *object_path,
                     const gchar      *interface_name,
                     const gchar      *property_name,
                     GError          **error,
                     gpointer          user_data);
                     
    static gboolean handle_set_property (GDBusConnection  *connection,
                     const gchar      *sender,
                     const gchar      *object_path,
                     const gchar      *interface_name,
                     const gchar      *property_name,
                     GVariant         *value,
                     GError          **error,
                     gpointer          user_data);


    constexpr static const GDBusInterfaceVTable interface_vtable =
    {
        handle_method_call,
        handle_get_property,
        handle_set_property,
    };



    gint owned_id;

    inline static const std::string server_bus_name {"org.mpris.MediaPlayer2.comet"};

    static void on_name_acquired(GDBusConnection* connection, const gchar* name, gpointer user_data);
    static void on_bus_acquired(GDBusConnection* connection, const gchar* name, gpointer user_data);


    public:
        void run();
        void stop();
       
        mpris_handler(mpris_signal_handler* signal_handler);

        static void emit_property_changed();

};

}
