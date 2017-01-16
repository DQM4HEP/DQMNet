/// \file ServiceHandler.h
/*
 *
 * ServiceHandler.h header template automatically generated by a class generator
 * Creation date : dim. d�c. 4 2016
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


#ifndef SERVICEHANDLER_H
#define SERVICEHANDLER_H

#include "json/json.h"

#include "dic.hxx"

#include "dqm4hep/DQMNet.h"

namespace dqm4hep {

  namespace net {

    class Client;
    template <typename S, typename U>
    class ServiceHandlerT;

    /** BaseServiceHandler class
     */
    class BaseServiceHandler
    {
      friend class Client;
    public:
      /** Get the request type
       */
      const std::string &getType() const;

      /** Get the request name
       */
      const std::string &getName() const;

      /** Get the request full name, as it is allocated on the network
       */
      const std::string &getFullName() const;

      /** Get the client interface
       */
      Client *getClient() const;

    protected:
      /** Constructor
       */
      BaseServiceHandler(Client *pClient, const std::string &type, const std::string &name);

      /** Destructor
       */
      virtual ~BaseServiceHandler();

    private:
      std::string                    m_type;             ///< The request handler type
      std::string                    m_name;             ///< The request handler name
      std::string                    m_fullName;         ///< The request handler full name
      Client                        *m_pClient;
    };

    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------

    template <typename T>
    class ServiceHandler : public BaseServiceHandler
    {
      friend class Client;
      template <typename S, typename U> friend class ServiceHandlerT;
    public:
      /**
       */
      virtual void serviceHandler(const T &value) = 0;

    private:
      /** ServiceInfo class.
       *
       *  The concrete dim updated info implementation
       */
      class ServiceInfo : public DimUpdatedInfo
      {
      public:
        /** Contructor
         */
        ServiceInfo(ServiceHandler *pHandler);

        /** The dim rpc handler
         */
        void infoHandler();

      private:
        ServiceHandler<T>        *m_pHandler;
      };

      /** Process the dim rpc request
       */
      void processService(ServiceInfo *pInfo);

    private:
      ServiceHandler(Client *pClient, const std::string &type, const std::string &name);
      virtual ~ServiceHandler();

    private:
      ServiceInfo                    m_serviceInfo;
    };

    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline ServiceHandler<T>::ServiceInfo::ServiceInfo(ServiceHandler<T> *pHandler) :
        DimUpdatedInfo(const_cast<char*>(pHandler->getFullName().c_str()), const_cast<char*>(std::string("{}").c_str())),
        m_pHandler(pHandler)
    {
      /* nop */
    }

    //-------------------------------------------------------------------------------------------------

    template <>
    inline ServiceHandler<int>::ServiceInfo::ServiceInfo(ServiceHandler<int> *pHandler) :
        DimUpdatedInfo(const_cast<char*>(pHandler->getFullName().c_str()), int(0)),
        m_pHandler(pHandler)
    {
      /* nop */
    }

    //-------------------------------------------------------------------------------------------------

    template <>
    inline ServiceHandler<double>::ServiceInfo::ServiceInfo(ServiceHandler<double> *pHandler) :
        DimUpdatedInfo(const_cast<char*>(pHandler->getFullName().c_str()), double(0.)),
        m_pHandler(pHandler)
    {
      /* nop */
    }

    //-------------------------------------------------------------------------------------------------

    template <>
    inline ServiceHandler<float>::ServiceInfo::ServiceInfo(ServiceHandler<float> *pHandler) :
        DimUpdatedInfo(const_cast<char*>(pHandler->getFullName().c_str()), float(0.)),
        m_pHandler(pHandler)
    {
      /* nop */
    }

    //-------------------------------------------------------------------------------------------------

    template <>
    inline ServiceHandler<std::string>::ServiceInfo::ServiceInfo(ServiceHandler<std::string> *pHandler) :
        DimUpdatedInfo(const_cast<char*>(pHandler->getFullName().c_str()), (char*)""),
        m_pHandler(pHandler)
    {
      /* nop */
    }

    //-------------------------------------------------------------------------------------------------

    template <>
    inline ServiceHandler<Json::Value>::ServiceInfo::ServiceInfo(ServiceHandler<Json::Value> *pHandler) :
        DimUpdatedInfo(const_cast<char*>(pHandler->getFullName().c_str()), (char*)""),
        m_pHandler(pHandler)
    {
      /* nop */
    }

    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline ServiceHandler<T>::ServiceHandler(Client *pClient, const std::string &type, const std::string &name) :
      BaseServiceHandler(pClient, type, name),
      m_serviceInfo(this)
    {
      /* nop */
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline ServiceHandler<T>::~ServiceHandler()
    {
      /* nop */
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline void ServiceHandler<T>::ServiceInfo::infoHandler()
    {
      m_pHandler->processService(this);
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline void ServiceHandler<T>::processService(ServiceInfo *pInfo)
    {
      throw std::runtime_error("ServiceHandler<T>::processService(): unknown service type");
    }

    //-------------------------------------------------------------------------------------------------

    template <>
    inline void ServiceHandler<int>::processService(ServiceInfo *pInfo)
    {
      this->serviceHandler(pInfo->getInt());
    }

    //-------------------------------------------------------------------------------------------------

    template <>
    inline void ServiceHandler<float>::processService(ServiceInfo *pInfo)
    {
      this->serviceHandler(pInfo->getFloat());
    }

    //-------------------------------------------------------------------------------------------------

    template <>
    inline void ServiceHandler<double>::processService(ServiceInfo *pInfo)
    {
      this->serviceHandler(pInfo->getDouble());
    }

    //-------------------------------------------------------------------------------------------------

    template <>
    inline void ServiceHandler<std::string>::processService(ServiceInfo *pInfo)
    {
      this->serviceHandler(pInfo->getString());
    }

    //-------------------------------------------------------------------------------------------------

    template <>
    inline void ServiceHandler<Buffer>::processService(ServiceInfo *pInfo)
    {
      Buffer *buffer = (Buffer*) pInfo->getData();

      if(!buffer)
        return;

      if(buffer->m_bufferSize == 0 || buffer->m_pBuffer == 0)
        return;

      this->serviceHandler(*buffer);
    }

    //-------------------------------------------------------------------------------------------------

    template <>
    inline void ServiceHandler<Json::Value>::processService(ServiceInfo *pInfo)
    {
      Json::Reader reader;
      Json::Value serviceValue;

      reader.parse(pInfo->getString(), serviceValue);
      this->serviceHandler(serviceValue);
    }

    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------

    /**
     */
    template <typename T, typename S>
    class ServiceHandlerT : public ServiceHandler<T>
    {
      friend class Client;
    public:
      typedef void (S::*Function)(const T &value);

    private:
      /**
       */
      ServiceHandlerT(Client *pClient, const std::string &type, const std::string &name,
          S *pController, Function function);

      /** Destructor
       */
      ~ServiceHandlerT();

      /**
       */
      void serviceHandler(const T &value);

    private:
      S            *m_pController;
      Function      m_function;
    };

    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------

    template <typename T, typename S>
    inline ServiceHandlerT<T,S>::ServiceHandlerT(Client *pClient, const std::string &type, const std::string &name,
        S *pController, Function function) :
        ServiceHandler<T>(pClient, type, name),
        m_pController(pController),
        m_function(function)
    {
      /* nop */
    }

    template <typename T, typename S>
    inline ServiceHandlerT<T,S>::~ServiceHandlerT()
    {
      /* nop */
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T, typename S>
    inline void ServiceHandlerT<T,S>::serviceHandler(const T &value)
    {
      (m_pController->*m_function)(value);
    }

  }

}

#endif  //  SERVICEHANDLER_H