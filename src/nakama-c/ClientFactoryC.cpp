/*
 * Copyright 2019 The Nakama Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifdef BUILD_C_API

#include "nakama-c/ClientFactory.h"
#include "nakama-cpp/ClientFactory.h"
#include "realtime/Patch.h"

NAKAMA_NAMESPACE_BEGIN

static std::vector<Nakama::NClientPtr> g_clients;

static NClient createClient(const tNClientParameters* parameters, std::function<Nakama::NClientPtr (const Nakama::NClientParameters&)> creator)
{
    Nakama::NClientParameters cppClientParams;

    cppClientParams.host = parameters->host;
    cppClientParams.port = parameters->port;
    cppClientParams.serverKey = parameters->serverKey;
    cppClientParams.ssl = parameters->ssl;

    if (get_patcher_config().clientmode == unofficial && cppClientParams.host == "eu4.online.paradox-interactive.com") {
      info("intercepting createClient\n");
      client_settings settings = get_unofficial_client_settings();
      cppClientParams.host = settings.host;
      cppClientParams.port = settings.port;
      cppClientParams.serverKey = settings.key;
      cppClientParams.ssl = settings.ssl;
      info("settings:\n");
      info("  host: %s\n", cppClientParams.host.c_str());
      info("  port: %d\n", cppClientParams.port);
      info("  key:  %s\n", cppClientParams.serverKey.c_str());
      info("  ssl:  %d\n", cppClientParams.ssl);
    }

    auto cppClient = creator(cppClientParams);

    if (cppClient)
    {
        g_clients.push_back(cppClient);
    }

    return (NClient)cppClient.get();
}

NAKAMA_NAMESPACE_END

extern "C" {

using namespace NAKAMA_NAMESPACE;

NClient createDefaultNakamaClient(const tNClientParameters* parameters)
{
    return createClient(parameters, [](const Nakama::NClientParameters& cppClientParams) { return Nakama::createDefaultClient(cppClientParams); });
}

NClient createGrpcNakamaClient(const tNClientParameters* parameters)
{
    return createClient(parameters, [](const Nakama::NClientParameters& cppClientParams) { return Nakama::createGrpcClient(cppClientParams); });
}

NClient createRestNakamaClient(const tNClientParameters* parameters)
{
    return createClient(parameters, [](const Nakama::NClientParameters& cppClientParams) { return Nakama::createRestClient(cppClientParams); });
}

void destroyNakamaClient(NClient client)
{
    if (client)
    {
        for (auto it = g_clients.begin(); it != g_clients.end(); ++it)
        {
            if ((NClient)it->get() == client)
            {
                g_clients.erase(it);
                break;
            }
        }
    }
}

} // extern "C"

#endif // BUILD_C_API
