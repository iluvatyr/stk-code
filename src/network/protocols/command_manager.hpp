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

#ifndef COMMAND_MANAGER_HPP
#define COMMAND_MANAGER_HPP

#include "irrString.h"

#include <algorithm>
#include <array>
#include <atomic>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <deque>
#include <vector>

#ifdef ENABLE_SQLITE3
#include <sqlite3.h>
#endif

class ServerLobby;
class Event;
class STKPeer;

class CommandManager
{
    enum CommandScope: int {
        CS_ALWAYS = 1,
        CS_SOCCER_TOURNAMENT = 2
        // add more powers of two if needed
    };

    struct Context
    {
        Event* m_event;

        std::shared_ptr<STKPeer> m_peer;

        std::vector<std::string> m_argv;

        std::string m_cmd;

        int m_user_permissions;

        Context(Event* event, std::shared_ptr<STKPeer> peer,
            std::vector<std::string>& argv, std::string& cmd,
            int user_permissions):
                m_event(event), m_peer(peer), m_argv(argv),
                m_cmd(cmd), m_user_permissions(user_permissions) {}
    };

    struct Command
    {
        std::string m_name;

        int m_permissions;

        void (CommandManager::*m_action)(Context& context);

        int m_scope;

        Command(std::string name,
            void (CommandManager::*f)(Context& context), int permissions, int scope = CS_ALWAYS):
                m_name(name), m_permissions(permissions), m_action(f), m_scope(scope) {}
    };

private:

    ServerLobby* m_lobby;

    std::vector<Command> m_commands;

    std::map<std::string, std::string> m_text_response;

    void initCommands();

    int getCurrentScope();

    bool isAvailable(const Command& c);

    void process_text(Context& context);
    void process_commands(Context& context);
    void process_replay(Context& context);
    void process_start(Context& context);
    void process_config(Context& context);
    void process_spectate(Context& context);
    void process_addons(Context& context);
    void process_lsa(Context& context);
    void process_pha(Context& context);
    void process_kick(Context& context);
    void process_unban(Context& context);
    void process_ban(Context& context);
    void process_pas(Context& context);
    void process_sha(Context& context);
    void process_mute(Context& context);
    void process_unmute(Context& context);
    void process_listmute(Context& context);
    void process_gnu(Context& context);
    void process_nognu(Context& context);
    void process_tell(Context& context);
    void process_standings(Context& context);
    void process_teamchat(Context& context);
    void process_to(Context& context);
    void process_public(Context& context);
    void process_record(Context& context);
    void process_power(Context& context);
    void process_length(Context& context);
    void process_queue(Context& context);
    void process_adminstart(Context& context);
    void process_shuffle(Context& context);
    void process_timeout(Context& context);
    void process_team(Context& context);
    void process_cat(Context& context);
    void process_troll(Context& context);
    void process_hitmsg(Context& context);
    void process_teamhit(Context& context);
    void process_register(Context& context);
#ifdef ENABLE_WEB_SUPPORT
    void process_token(Context& context);
#endif
    // soccer tournament commands
    void process_muteall(Context& context);
    void process_game(Context& context);
    void process_role(Context& context);
    void process_stop(Context& context);
    void process_go(Context& context);
    void process_lobby(Context& context);
    void process_init(Context& context);
    void process_mimiz(Context& context);

public:

    CommandManager(ServerLobby* lobby = nullptr);

    void handleCommand(Event* event, std::shared_ptr<STKPeer> peer);

    bool isInitialized() { return m_lobby != nullptr; }

    template<typename T>
    void addTextResponse(std::string key, T&& value)
                                              { m_text_response[key] = value; }
};

#endif // COMMAND_MANAGER_HPP