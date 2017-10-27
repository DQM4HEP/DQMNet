  /// \file dqm4hep-server-info.cc
/*
 *
 * test-server.cc source template automatically generated by a class generator
 * Creation date : sam. d�c. 3 2016
 *
 * This file is part of DQM4HEP libraries.
 *
 * DQM4HEP is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * based upon these libraries are permitted. Any copy of these libraries
 * must include this copyright notice.
 *
 * DQM4HEP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with DQM4HEP.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @author Remi Ete
 * @copyright CNRS , IPNL
 */


#include "dqm4hep/Client.h"

#include "json/json.h"

using namespace dqm4hep::net;

int main(int argc, char **argv)
{
  if(argc != 2)
  {
    std::cout << "Usage : dqm4hep-server-info servername" << std::endl;
    return 1;
  }

  std::string serverName(argv[1]);

  Client client;
  Json::Value serverInfo;
  client.queryServerInfo(serverName, serverInfo);

  Json::StreamWriterBuilder builder;
  builder["indentation"] = "  ";
  std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
  std::ostringstream jsonResponse;
  writer->write(serverInfo, &jsonResponse);

  std::cout << jsonResponse.str() << std::endl;

  return 0;
}
