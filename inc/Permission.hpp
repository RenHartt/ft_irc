#include <iostream>
#include <map>
#include <stdint.h>

struct Rights {
    /* uint8_t write : 1; */
	uint8_t isOperator : 1;
    /* uint8_t     uploadImg : 1; */

    /* struct { */
    /*     uint8_t mute : 1; */
    /*     uint8_t kick : 1; */
    /*     uint8_t ban : 1; */
    /*     uint8_t invite : 1; */
    /*     uint8_t setPassWord : 1; */
    /*     uint8_t changeChannelVisibility : 1; */
    /*     uint8_t changeChannelName : 1; */
    /*     uint8_t changeGrantable : 1; */
    /*     uint8_t caca : 1; */
    /* } admin; */
};

struct ClientRight {
    Rights rights;
    Rights grantable;
};

typedef void (*SetPermission)(const ClientRight &, ClientRight &, bool);
typedef std::map<std::string, SetPermission> MapSetPermission;

inline void err_message(const char *str) { std::cerr << str << std::endl; }

#define SET_PERMISSION_FUNC(permission)                                                            \
    void set_permission_##permission(const ClientRight &grantor, ClientRight &target, bool state)  \
    {                                                                                              \
        if (grantor.grantable.permission)                                                          \
            target.rights.permission = state;                                                      \
        else                                                                                       \
            err_message("rights." #permission);                                                    \
    }

#define SET_PERMISSION_ADMIN_FUNC(permission)                                                      \
    void set_permission_##permission(const ClientRight &grantor, ClientRight &target, bool state)  \
    {                                                                                              \
        if (grantor.grantable.admin.permission)                                                    \
            target.rights.admin.permission = state;                                                \
        else                                                                                       \
            err_message("rights." #permission);                                                    \
    }

#define SET_PERMISSION_SETMAP(value) SET_MAP[#value] = set_permission_##value;
