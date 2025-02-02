#include "Logic.h"
#include <libultraship/libultraship.h>

extern "C" {
#include "variables.h"
#include "ShipUtils.h"
}

namespace Rando {

namespace Logic {

void ApplyNoLogicToSaveContext(std::unordered_map<RandoCheckId, bool>& checkPool, std::vector<RandoItemId>& itemPool) {
    for (size_t i = 0; i < itemPool.size(); i++) {
        std::swap(itemPool[i], itemPool[Ship_Random(0, itemPool.size() - 1)]);
    }

    if (RANDO_SAVE_OPTIONS[RO_SHUFFLE_ENEMY_SOULS] == RO_GENERIC_YES) {
        uint32_t index = RI_SOUL_ALIEN;
        for (auto& item : itemPool) {
    
            if (item == RI_RUPEE_RED) {
                item = (RandoItemId)index;
                index++;
            }
            if ((RandoItemId)index > RI_SOUL_WOLFOS) {
                break;
            }
        }
    }

    for (auto& [randoCheckId, _] : checkPool) {
        if (randoCheckId == RC_UNKNOWN) {
            continue;
        }

        RANDO_SAVE_CHECKS[randoCheckId].shuffled = true;
        RANDO_SAVE_CHECKS[randoCheckId].randoItemId = itemPool.back();
        itemPool.pop_back();
    }
}

} // namespace Logic

} // namespace Rando
