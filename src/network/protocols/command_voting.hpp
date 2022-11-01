//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2021 kimden
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef COMMAND_VOTING_HPP
#define COMMAND_VOTING_HPP

#include "irrString.h"

#include <algorithm>
#include <atomic>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <deque>
#include <vector>

class CommandVoting
{
    double m_threshold;
    bool m_need_check = false;
    std::map<std::string, std::string> m_selected_options;
    // If not -1, all commands voted from this votable will be merged into one,
    // removing common prefix of length m_merge from all category names but one
    int m_merge = -1;
private:
    std::map<std::string, std::map<std::string, std::set<std::string>>> m_votes_by_poll;
    std::map<std::string, std::map<std::string, std::string>> m_votes_by_player;
    std::map<std::string, double> m_custom_thresholds;
public:
    static const double DEFAULT_THRESHOLD;
    CommandVoting(double threshold = DEFAULT_THRESHOLD);
    void setCustomThreshold(std::string category, double value)
                                     { m_custom_thresholds[category] = value; }
    void resetCustomThreshold(std::string category)
                                       { m_custom_thresholds.erase(category); }
    bool needsCheck() { return m_need_check; }
    void castVote(std::string player, std::string category, std::string vote);
    void uncastVote(std::string player, std::string category);
    std::pair<std::map<std::string, int>, std::map<std::string, std::string>>
        process(std::multiset<std::string>& all_users);
    std::string getAnyBest(std::string category);
    void reset(std::string category);
    void resetAllVotes();
    void setMerge(int value = -1)                           { m_merge = value; }
};

#endif // COMMAND_VOTING_HPP