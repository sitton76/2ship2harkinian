#include "StaticData.h"

namespace Rando {

namespace StaticData {

#define RO(id)                             \
    {                                      \
        id, {                              \
            id, #id, "gRando.Options." #id \
        }                                  \
    }

// clang-format off
std::map<RandoOptionId, RandoStaticOption> Options = {
    RO(RO_LOGIC),
    RO(RO_SHUFFLE_GOLD_SKULLTULAS),
    RO(RO_SHUFFLE_OWL_STATUES),
    RO(RO_SHUFFLE_MUNDANE),
    RO(RO_SHUFFLE_SHOPS),
    RO(RO_SHUFFLE_BOSS_REMAINS),
};
// clang-format on

RandoOptionId GetOptionIdFromName(const char* name) {
    for (auto& [randoOptionId, randoStaticOption] : Options) {
        if (strcmp(name, randoStaticOption.name) == 0) {
            return randoOptionId;
        }
    }
    return RO_MAX;
}

} // namespace StaticData

} // namespace Rando
