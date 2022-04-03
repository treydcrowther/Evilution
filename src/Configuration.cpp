//
// Created by kdex9 on 3/24/2022.
//

#include "Configuration.h"
Configuration::Configuration(OrgConfig org, BoardConfig board, SimConfig sim)
{
    orgConfig = org;
    boardConfig = board;
    simConfig = sim;
}

OrgConfig Configuration::GetOrgConfig() {
    return orgConfig;
}

BoardConfig Configuration::GetBoardConfig() {
    return boardConfig;
}

SimConfig Configuration::GetSimConfig() {
    return simConfig;
}
