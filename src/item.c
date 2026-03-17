#include "item.h"

Item InitItem(ITEM_ID id)
{
    switch (id)
    {
        case ITEM_NONE:
        return (Item){
            id,
            "<None>",
            TL_GENERAL,
            (Vector2){1,0},
            GRAY,
            ES_EVERY,
            CreateEmptyStatBonuses(),
        };
        case ITEM_TEST:
        return (Item){
            id,
            "Test Item",
            TL_ITEMS,
            (Vector2){0,0},
            GRAY,
            ES_EVERY,
            CreateEmptyStatBonuses(),
        };
    }
}