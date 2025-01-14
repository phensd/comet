/*
 * This file is generated by gdbus-codegen, do not modify it.
 *
 * The license of this code is the same as for the D-Bus interface description
 * it was derived from. Note that it links to GLib, so must comply with the
 * LGPL linking clauses.
 */

#ifndef __MPRIS_H__
#define __MPRIS_H__

#include <gio/gio.h>

G_BEGIN_DECLS


/* ------------------------------------------------------------------------ */
/* Declarations for org.mpris.MediaPlayer2.Player */

#define COMET_TYPE_MEDIA_PLAYER2_PLAYER (comet_media_player2_player_get_type ())
#define COMET_MEDIA_PLAYER2_PLAYER(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), COMET_TYPE_MEDIA_PLAYER2_PLAYER, cometMediaPlayer2Player))
#define COMET_IS_MEDIA_PLAYER2_PLAYER(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), COMET_TYPE_MEDIA_PLAYER2_PLAYER))
#define COMET_MEDIA_PLAYER2_PLAYER_GET_IFACE(o) (G_TYPE_INSTANCE_GET_INTERFACE ((o), COMET_TYPE_MEDIA_PLAYER2_PLAYER, cometMediaPlayer2PlayerIface))

struct _cometMediaPlayer2Player;
typedef struct _cometMediaPlayer2Player cometMediaPlayer2Player;
typedef struct _cometMediaPlayer2PlayerIface cometMediaPlayer2PlayerIface;

struct _cometMediaPlayer2PlayerIface
{
  GTypeInterface parent_iface;


  gboolean (*handle_play) (
    cometMediaPlayer2Player *object,
    GDBusMethodInvocation *invocation);

  gboolean (*handle_play_pause) (
    cometMediaPlayer2Player *object,
    GDBusMethodInvocation *invocation);

  gdouble  (*get_volume) (cometMediaPlayer2Player *object);

};

GType comet_media_player2_player_get_type (void) G_GNUC_CONST;

GDBusInterfaceInfo *comet_media_player2_player_interface_info (void);
guint comet_media_player2_player_override_properties (GObjectClass *klass, guint property_id_begin);


/* D-Bus method call completion functions: */
void comet_media_player2_player_complete_play (
    cometMediaPlayer2Player *object,
    GDBusMethodInvocation *invocation);

void comet_media_player2_player_complete_play_pause (
    cometMediaPlayer2Player *object,
    GDBusMethodInvocation *invocation);



/* D-Bus method calls: */
void comet_media_player2_player_call_play (
    cometMediaPlayer2Player *proxy,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean comet_media_player2_player_call_play_finish (
    cometMediaPlayer2Player *proxy,
    GAsyncResult *res,
    GError **error);

gboolean comet_media_player2_player_call_play_sync (
    cometMediaPlayer2Player *proxy,
    GCancellable *cancellable,
    GError **error);

void comet_media_player2_player_call_play_pause (
    cometMediaPlayer2Player *proxy,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean comet_media_player2_player_call_play_pause_finish (
    cometMediaPlayer2Player *proxy,
    GAsyncResult *res,
    GError **error);

gboolean comet_media_player2_player_call_play_pause_sync (
    cometMediaPlayer2Player *proxy,
    GCancellable *cancellable,
    GError **error);



/* D-Bus property accessors: */
gdouble comet_media_player2_player_get_volume (cometMediaPlayer2Player *object);
void comet_media_player2_player_set_volume (cometMediaPlayer2Player *object, gdouble value);


/* ---- */

#define COMET_TYPE_MEDIA_PLAYER2_PLAYER_PROXY (comet_media_player2_player_proxy_get_type ())
#define COMET_MEDIA_PLAYER2_PLAYER_PROXY(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), COMET_TYPE_MEDIA_PLAYER2_PLAYER_PROXY, cometMediaPlayer2PlayerProxy))
#define COMET_MEDIA_PLAYER2_PLAYER_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), COMET_TYPE_MEDIA_PLAYER2_PLAYER_PROXY, cometMediaPlayer2PlayerProxyClass))
#define COMET_MEDIA_PLAYER2_PLAYER_PROXY_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), COMET_TYPE_MEDIA_PLAYER2_PLAYER_PROXY, cometMediaPlayer2PlayerProxyClass))
#define COMET_IS_MEDIA_PLAYER2_PLAYER_PROXY(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), COMET_TYPE_MEDIA_PLAYER2_PLAYER_PROXY))
#define COMET_IS_MEDIA_PLAYER2_PLAYER_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), COMET_TYPE_MEDIA_PLAYER2_PLAYER_PROXY))

typedef struct _cometMediaPlayer2PlayerProxy cometMediaPlayer2PlayerProxy;
typedef struct _cometMediaPlayer2PlayerProxyClass cometMediaPlayer2PlayerProxyClass;
typedef struct _cometMediaPlayer2PlayerProxyPrivate cometMediaPlayer2PlayerProxyPrivate;

struct _cometMediaPlayer2PlayerProxy
{
  /*< private >*/
  GDBusProxy parent_instance;
  cometMediaPlayer2PlayerProxyPrivate *priv;
};

struct _cometMediaPlayer2PlayerProxyClass
{
  GDBusProxyClass parent_class;
};

GType comet_media_player2_player_proxy_get_type (void) G_GNUC_CONST;

#if GLIB_CHECK_VERSION(2, 44, 0)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (cometMediaPlayer2PlayerProxy, g_object_unref)
#endif

void comet_media_player2_player_proxy_new (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
cometMediaPlayer2Player *comet_media_player2_player_proxy_new_finish (
    GAsyncResult        *res,
    GError             **error);
cometMediaPlayer2Player *comet_media_player2_player_proxy_new_sync (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);

void comet_media_player2_player_proxy_new_for_bus (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
cometMediaPlayer2Player *comet_media_player2_player_proxy_new_for_bus_finish (
    GAsyncResult        *res,
    GError             **error);
cometMediaPlayer2Player *comet_media_player2_player_proxy_new_for_bus_sync (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);


/* ---- */

#define COMET_TYPE_MEDIA_PLAYER2_PLAYER_SKELETON (comet_media_player2_player_skeleton_get_type ())
#define COMET_MEDIA_PLAYER2_PLAYER_SKELETON(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), COMET_TYPE_MEDIA_PLAYER2_PLAYER_SKELETON, cometMediaPlayer2PlayerSkeleton))
#define COMET_MEDIA_PLAYER2_PLAYER_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), COMET_TYPE_MEDIA_PLAYER2_PLAYER_SKELETON, cometMediaPlayer2PlayerSkeletonClass))
#define COMET_MEDIA_PLAYER2_PLAYER_SKELETON_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), COMET_TYPE_MEDIA_PLAYER2_PLAYER_SKELETON, cometMediaPlayer2PlayerSkeletonClass))
#define COMET_IS_MEDIA_PLAYER2_PLAYER_SKELETON(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), COMET_TYPE_MEDIA_PLAYER2_PLAYER_SKELETON))
#define COMET_IS_MEDIA_PLAYER2_PLAYER_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), COMET_TYPE_MEDIA_PLAYER2_PLAYER_SKELETON))

typedef struct _cometMediaPlayer2PlayerSkeleton cometMediaPlayer2PlayerSkeleton;
typedef struct _cometMediaPlayer2PlayerSkeletonClass cometMediaPlayer2PlayerSkeletonClass;
typedef struct _cometMediaPlayer2PlayerSkeletonPrivate cometMediaPlayer2PlayerSkeletonPrivate;

struct _cometMediaPlayer2PlayerSkeleton
{
  /*< private >*/
  GDBusInterfaceSkeleton parent_instance;
  cometMediaPlayer2PlayerSkeletonPrivate *priv;
};

struct _cometMediaPlayer2PlayerSkeletonClass
{
  GDBusInterfaceSkeletonClass parent_class;
};

GType comet_media_player2_player_skeleton_get_type (void) G_GNUC_CONST;

#if GLIB_CHECK_VERSION(2, 44, 0)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (cometMediaPlayer2PlayerSkeleton, g_object_unref)
#endif

cometMediaPlayer2Player *comet_media_player2_player_skeleton_new (void);


G_END_DECLS

#endif /* __MPRIS_H__ */
