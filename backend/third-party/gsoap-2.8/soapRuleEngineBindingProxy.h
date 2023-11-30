/* soapRuleEngineBindingProxy.h
   Generated by gSOAP 2.8.131 for onvif.h

gSOAP XML Web services tools
Copyright (C) 2000-2023, Robert van Engelen, Genivia Inc. All Rights Reserved.
The soapcpp2 tool and its generated software are released under the GPL.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
--------------------------------------------------------------------------------
A commercial use license is available from Genivia Inc., contact@genivia.com
--------------------------------------------------------------------------------
*/

#ifndef soapRuleEngineBindingProxy_H
#define soapRuleEngineBindingProxy_H
#include "soapH.h"

    class SOAP_CMAC RuleEngineBindingProxy {
      public:
        /// Context to manage proxy IO and data
        struct soap *soap;
        /// flag indicating that this context is owned by this proxy and should be deleted by the destructor
        bool soap_own;
        /// Endpoint URL of service 'RuleEngineBindingProxy' (change as needed)
        const char *soap_endpoint;
        /// Variables globally declared in onvif.h, if any
        /// Construct a proxy with new managing context
        RuleEngineBindingProxy();
        /// Copy constructor
        RuleEngineBindingProxy(const RuleEngineBindingProxy& rhs);
        /// Construct proxy given a shared managing context
        RuleEngineBindingProxy(struct soap*);
        /// Construct proxy given a shared managing context and endpoint URL
        RuleEngineBindingProxy(struct soap*, const char *soap_endpoint_url);
        /// Constructor taking an endpoint URL
        RuleEngineBindingProxy(const char *soap_endpoint_url);
        /// Constructor taking input and output mode flags for the new managing context
        RuleEngineBindingProxy(soap_mode iomode);
        /// Constructor taking endpoint URL and input and output mode flags for the new managing context
        RuleEngineBindingProxy(const char *soap_endpoint_url, soap_mode iomode);
        /// Constructor taking input and output mode flags for the new managing context
        RuleEngineBindingProxy(soap_mode imode, soap_mode omode);
        /// Destructor deletes deserialized data and its managing context, when the context was allocated by the constructor
        virtual ~RuleEngineBindingProxy();
        /// Initializer used by constructors
        virtual void RuleEngineBindingProxy_init(soap_mode imode, soap_mode omode);
        /// Return a copy that has a new managing context with the same engine state
        virtual RuleEngineBindingProxy *copy();
        /// Copy assignment
        RuleEngineBindingProxy& operator=(const RuleEngineBindingProxy&);
        /// Delete all deserialized data (uses soap_destroy() and soap_end())
        virtual void destroy();
        /// Delete all deserialized data and reset to default
        virtual void reset();
        /// Disables and removes SOAP Header from message by setting soap->header = NULL
        virtual void soap_noheader();
        /// Add SOAP Header to message
        virtual void soap_header(char *wsa__MessageID, struct wsa__Relationship *wsa__RelatesTo, struct wsa__EndpointReferenceType *wsa__From, struct wsa__EndpointReferenceType *wsa__ReplyTo, struct wsa__EndpointReferenceType *wsa__FaultTo, char *wsa__To, char *wsa__Action, struct wsdd__AppSequenceType *wsdd__AppSequence, char *wsa5__MessageID, struct wsa5__RelatesToType *wsa5__RelatesTo, struct wsa5__EndpointReferenceType *wsa5__From, struct wsa5__EndpointReferenceType *wsa5__ReplyTo, struct wsa5__EndpointReferenceType *wsa5__FaultTo, char *wsa5__To, char *wsa5__Action, struct chan__ChannelInstanceType *chan__ChannelInstance);
        /// Get SOAP Header structure (i.e. soap->header, which is NULL when absent)
        virtual ::SOAP_ENV__Header *soap_header();
        /// Get SOAP Fault structure (i.e. soap->fault, which is NULL when absent)
        virtual ::SOAP_ENV__Fault *soap_fault();
        /// Get SOAP Fault subcode QName string (NULL when absent)
        virtual const char *soap_fault_subcode();
        /// Get SOAP Fault string/reason (NULL when absent)
        virtual const char *soap_fault_string();
        /// Get SOAP Fault detail XML string (NULL when absent)
        virtual const char *soap_fault_detail();
        /// Close connection (normally automatic, except for send_X ops)
        virtual int soap_close_socket();
        /// Force close connection (can kill a thread blocked on IO)
        virtual int soap_force_close_socket();
        /// Print fault
        virtual void soap_print_fault(FILE*);
    #ifndef WITH_LEAN
    #ifndef WITH_COMPAT
        /// Print fault to stream
        virtual void soap_stream_fault(std::ostream&);
    #endif
        /// Write fault to buffer
        virtual char *soap_sprint_fault(char *buf, size_t len);
    #endif
        //
        /// Web service synchronous operation 'GetSupportedRules' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetSupportedRules(_tan__GetSupportedRules *tan__GetSupportedRules, _tan__GetSupportedRulesResponse &tan__GetSupportedRulesResponse) { return this->GetSupportedRules(NULL, NULL, tan__GetSupportedRules, tan__GetSupportedRulesResponse); }
        /// Web service synchronous operation 'GetSupportedRules' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetSupportedRules(const char *soap_endpoint_url, const char *soap_action, _tan__GetSupportedRules *tan__GetSupportedRules, _tan__GetSupportedRulesResponse &tan__GetSupportedRulesResponse) { return this->send_GetSupportedRules(soap_endpoint_url, soap_action, tan__GetSupportedRules) || this->recv_GetSupportedRules(tan__GetSupportedRulesResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetSupportedRules' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetSupportedRules(const char *soap_endpoint_url, const char *soap_action, _tan__GetSupportedRules *tan__GetSupportedRules);
        /// Web service asynchronous operation 'recv_GetSupportedRules' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetSupportedRules(_tan__GetSupportedRulesResponse &tan__GetSupportedRulesResponse);
        //
        /// Web service synchronous operation 'CreateRules' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int CreateRules(_tan__CreateRules *tan__CreateRules, _tan__CreateRulesResponse &tan__CreateRulesResponse) { return this->CreateRules(NULL, NULL, tan__CreateRules, tan__CreateRulesResponse); }
        /// Web service synchronous operation 'CreateRules' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int CreateRules(const char *soap_endpoint_url, const char *soap_action, _tan__CreateRules *tan__CreateRules, _tan__CreateRulesResponse &tan__CreateRulesResponse) { return this->send_CreateRules(soap_endpoint_url, soap_action, tan__CreateRules) || this->recv_CreateRules(tan__CreateRulesResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_CreateRules' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_CreateRules(const char *soap_endpoint_url, const char *soap_action, _tan__CreateRules *tan__CreateRules);
        /// Web service asynchronous operation 'recv_CreateRules' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_CreateRules(_tan__CreateRulesResponse &tan__CreateRulesResponse);
        //
        /// Web service synchronous operation 'DeleteRules' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int DeleteRules(_tan__DeleteRules *tan__DeleteRules, _tan__DeleteRulesResponse &tan__DeleteRulesResponse) { return this->DeleteRules(NULL, NULL, tan__DeleteRules, tan__DeleteRulesResponse); }
        /// Web service synchronous operation 'DeleteRules' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int DeleteRules(const char *soap_endpoint_url, const char *soap_action, _tan__DeleteRules *tan__DeleteRules, _tan__DeleteRulesResponse &tan__DeleteRulesResponse) { return this->send_DeleteRules(soap_endpoint_url, soap_action, tan__DeleteRules) || this->recv_DeleteRules(tan__DeleteRulesResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_DeleteRules' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_DeleteRules(const char *soap_endpoint_url, const char *soap_action, _tan__DeleteRules *tan__DeleteRules);
        /// Web service asynchronous operation 'recv_DeleteRules' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_DeleteRules(_tan__DeleteRulesResponse &tan__DeleteRulesResponse);
        //
        /// Web service synchronous operation 'GetRules' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetRules(_tan__GetRules *tan__GetRules, _tan__GetRulesResponse &tan__GetRulesResponse) { return this->GetRules(NULL, NULL, tan__GetRules, tan__GetRulesResponse); }
        /// Web service synchronous operation 'GetRules' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetRules(const char *soap_endpoint_url, const char *soap_action, _tan__GetRules *tan__GetRules, _tan__GetRulesResponse &tan__GetRulesResponse) { return this->send_GetRules(soap_endpoint_url, soap_action, tan__GetRules) || this->recv_GetRules(tan__GetRulesResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetRules' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetRules(const char *soap_endpoint_url, const char *soap_action, _tan__GetRules *tan__GetRules);
        /// Web service asynchronous operation 'recv_GetRules' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetRules(_tan__GetRulesResponse &tan__GetRulesResponse);
        //
        /// Web service synchronous operation 'GetRuleOptions' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetRuleOptions(_tan__GetRuleOptions *tan__GetRuleOptions, _tan__GetRuleOptionsResponse &tan__GetRuleOptionsResponse) { return this->GetRuleOptions(NULL, NULL, tan__GetRuleOptions, tan__GetRuleOptionsResponse); }
        /// Web service synchronous operation 'GetRuleOptions' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetRuleOptions(const char *soap_endpoint_url, const char *soap_action, _tan__GetRuleOptions *tan__GetRuleOptions, _tan__GetRuleOptionsResponse &tan__GetRuleOptionsResponse) { return this->send_GetRuleOptions(soap_endpoint_url, soap_action, tan__GetRuleOptions) || this->recv_GetRuleOptions(tan__GetRuleOptionsResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetRuleOptions' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetRuleOptions(const char *soap_endpoint_url, const char *soap_action, _tan__GetRuleOptions *tan__GetRuleOptions);
        /// Web service asynchronous operation 'recv_GetRuleOptions' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetRuleOptions(_tan__GetRuleOptionsResponse &tan__GetRuleOptionsResponse);
        //
        /// Web service synchronous operation 'ModifyRules' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int ModifyRules(_tan__ModifyRules *tan__ModifyRules, _tan__ModifyRulesResponse &tan__ModifyRulesResponse) { return this->ModifyRules(NULL, NULL, tan__ModifyRules, tan__ModifyRulesResponse); }
        /// Web service synchronous operation 'ModifyRules' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int ModifyRules(const char *soap_endpoint_url, const char *soap_action, _tan__ModifyRules *tan__ModifyRules, _tan__ModifyRulesResponse &tan__ModifyRulesResponse) { return this->send_ModifyRules(soap_endpoint_url, soap_action, tan__ModifyRules) || this->recv_ModifyRules(tan__ModifyRulesResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_ModifyRules' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_ModifyRules(const char *soap_endpoint_url, const char *soap_action, _tan__ModifyRules *tan__ModifyRules);
        /// Web service asynchronous operation 'recv_ModifyRules' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_ModifyRules(_tan__ModifyRulesResponse &tan__ModifyRulesResponse);
        //
        /// Web service synchronous operation 'GetServiceCapabilities' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetServiceCapabilities(_tan__GetServiceCapabilities *tan__GetServiceCapabilities, _tan__GetServiceCapabilitiesResponse &tan__GetServiceCapabilitiesResponse) { return this->GetServiceCapabilities(NULL, NULL, tan__GetServiceCapabilities, tan__GetServiceCapabilitiesResponse); }
        /// Web service synchronous operation 'GetServiceCapabilities' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetServiceCapabilities(const char *soap_endpoint_url, const char *soap_action, _tan__GetServiceCapabilities *tan__GetServiceCapabilities, _tan__GetServiceCapabilitiesResponse &tan__GetServiceCapabilitiesResponse) { return this->send_GetServiceCapabilities(soap_endpoint_url, soap_action, tan__GetServiceCapabilities) || this->recv_GetServiceCapabilities(tan__GetServiceCapabilitiesResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetServiceCapabilities' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetServiceCapabilities(const char *soap_endpoint_url, const char *soap_action, _tan__GetServiceCapabilities *tan__GetServiceCapabilities);
        /// Web service asynchronous operation 'recv_GetServiceCapabilities' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetServiceCapabilities(_tan__GetServiceCapabilitiesResponse &tan__GetServiceCapabilitiesResponse);
        //
        /// Web service synchronous operation 'GetSupportedAnalyticsModules' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetSupportedAnalyticsModules(_tan__GetSupportedAnalyticsModules *tan__GetSupportedAnalyticsModules, _tan__GetSupportedAnalyticsModulesResponse &tan__GetSupportedAnalyticsModulesResponse) { return this->GetSupportedAnalyticsModules(NULL, NULL, tan__GetSupportedAnalyticsModules, tan__GetSupportedAnalyticsModulesResponse); }
        /// Web service synchronous operation 'GetSupportedAnalyticsModules' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetSupportedAnalyticsModules(const char *soap_endpoint_url, const char *soap_action, _tan__GetSupportedAnalyticsModules *tan__GetSupportedAnalyticsModules, _tan__GetSupportedAnalyticsModulesResponse &tan__GetSupportedAnalyticsModulesResponse) { return this->send_GetSupportedAnalyticsModules(soap_endpoint_url, soap_action, tan__GetSupportedAnalyticsModules) || this->recv_GetSupportedAnalyticsModules(tan__GetSupportedAnalyticsModulesResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetSupportedAnalyticsModules' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetSupportedAnalyticsModules(const char *soap_endpoint_url, const char *soap_action, _tan__GetSupportedAnalyticsModules *tan__GetSupportedAnalyticsModules);
        /// Web service asynchronous operation 'recv_GetSupportedAnalyticsModules' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetSupportedAnalyticsModules(_tan__GetSupportedAnalyticsModulesResponse &tan__GetSupportedAnalyticsModulesResponse);
        //
        /// Web service synchronous operation 'CreateAnalyticsModules' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int CreateAnalyticsModules(_tan__CreateAnalyticsModules *tan__CreateAnalyticsModules, _tan__CreateAnalyticsModulesResponse &tan__CreateAnalyticsModulesResponse) { return this->CreateAnalyticsModules(NULL, NULL, tan__CreateAnalyticsModules, tan__CreateAnalyticsModulesResponse); }
        /// Web service synchronous operation 'CreateAnalyticsModules' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int CreateAnalyticsModules(const char *soap_endpoint_url, const char *soap_action, _tan__CreateAnalyticsModules *tan__CreateAnalyticsModules, _tan__CreateAnalyticsModulesResponse &tan__CreateAnalyticsModulesResponse) { return this->send_CreateAnalyticsModules(soap_endpoint_url, soap_action, tan__CreateAnalyticsModules) || this->recv_CreateAnalyticsModules(tan__CreateAnalyticsModulesResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_CreateAnalyticsModules' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_CreateAnalyticsModules(const char *soap_endpoint_url, const char *soap_action, _tan__CreateAnalyticsModules *tan__CreateAnalyticsModules);
        /// Web service asynchronous operation 'recv_CreateAnalyticsModules' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_CreateAnalyticsModules(_tan__CreateAnalyticsModulesResponse &tan__CreateAnalyticsModulesResponse);
        //
        /// Web service synchronous operation 'DeleteAnalyticsModules' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int DeleteAnalyticsModules(_tan__DeleteAnalyticsModules *tan__DeleteAnalyticsModules, _tan__DeleteAnalyticsModulesResponse &tan__DeleteAnalyticsModulesResponse) { return this->DeleteAnalyticsModules(NULL, NULL, tan__DeleteAnalyticsModules, tan__DeleteAnalyticsModulesResponse); }
        /// Web service synchronous operation 'DeleteAnalyticsModules' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int DeleteAnalyticsModules(const char *soap_endpoint_url, const char *soap_action, _tan__DeleteAnalyticsModules *tan__DeleteAnalyticsModules, _tan__DeleteAnalyticsModulesResponse &tan__DeleteAnalyticsModulesResponse) { return this->send_DeleteAnalyticsModules(soap_endpoint_url, soap_action, tan__DeleteAnalyticsModules) || this->recv_DeleteAnalyticsModules(tan__DeleteAnalyticsModulesResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_DeleteAnalyticsModules' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_DeleteAnalyticsModules(const char *soap_endpoint_url, const char *soap_action, _tan__DeleteAnalyticsModules *tan__DeleteAnalyticsModules);
        /// Web service asynchronous operation 'recv_DeleteAnalyticsModules' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_DeleteAnalyticsModules(_tan__DeleteAnalyticsModulesResponse &tan__DeleteAnalyticsModulesResponse);
        //
        /// Web service synchronous operation 'GetAnalyticsModules' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetAnalyticsModules(_tan__GetAnalyticsModules *tan__GetAnalyticsModules, _tan__GetAnalyticsModulesResponse &tan__GetAnalyticsModulesResponse) { return this->GetAnalyticsModules(NULL, NULL, tan__GetAnalyticsModules, tan__GetAnalyticsModulesResponse); }
        /// Web service synchronous operation 'GetAnalyticsModules' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetAnalyticsModules(const char *soap_endpoint_url, const char *soap_action, _tan__GetAnalyticsModules *tan__GetAnalyticsModules, _tan__GetAnalyticsModulesResponse &tan__GetAnalyticsModulesResponse) { return this->send_GetAnalyticsModules(soap_endpoint_url, soap_action, tan__GetAnalyticsModules) || this->recv_GetAnalyticsModules(tan__GetAnalyticsModulesResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetAnalyticsModules' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetAnalyticsModules(const char *soap_endpoint_url, const char *soap_action, _tan__GetAnalyticsModules *tan__GetAnalyticsModules);
        /// Web service asynchronous operation 'recv_GetAnalyticsModules' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetAnalyticsModules(_tan__GetAnalyticsModulesResponse &tan__GetAnalyticsModulesResponse);
        //
        /// Web service synchronous operation 'GetAnalyticsModuleOptions' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetAnalyticsModuleOptions(_tan__GetAnalyticsModuleOptions *tan__GetAnalyticsModuleOptions, _tan__GetAnalyticsModuleOptionsResponse &tan__GetAnalyticsModuleOptionsResponse) { return this->GetAnalyticsModuleOptions(NULL, NULL, tan__GetAnalyticsModuleOptions, tan__GetAnalyticsModuleOptionsResponse); }
        /// Web service synchronous operation 'GetAnalyticsModuleOptions' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetAnalyticsModuleOptions(const char *soap_endpoint_url, const char *soap_action, _tan__GetAnalyticsModuleOptions *tan__GetAnalyticsModuleOptions, _tan__GetAnalyticsModuleOptionsResponse &tan__GetAnalyticsModuleOptionsResponse) { return this->send_GetAnalyticsModuleOptions(soap_endpoint_url, soap_action, tan__GetAnalyticsModuleOptions) || this->recv_GetAnalyticsModuleOptions(tan__GetAnalyticsModuleOptionsResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetAnalyticsModuleOptions' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetAnalyticsModuleOptions(const char *soap_endpoint_url, const char *soap_action, _tan__GetAnalyticsModuleOptions *tan__GetAnalyticsModuleOptions);
        /// Web service asynchronous operation 'recv_GetAnalyticsModuleOptions' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetAnalyticsModuleOptions(_tan__GetAnalyticsModuleOptionsResponse &tan__GetAnalyticsModuleOptionsResponse);
        //
        /// Web service synchronous operation 'ModifyAnalyticsModules' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int ModifyAnalyticsModules(_tan__ModifyAnalyticsModules *tan__ModifyAnalyticsModules, _tan__ModifyAnalyticsModulesResponse &tan__ModifyAnalyticsModulesResponse) { return this->ModifyAnalyticsModules(NULL, NULL, tan__ModifyAnalyticsModules, tan__ModifyAnalyticsModulesResponse); }
        /// Web service synchronous operation 'ModifyAnalyticsModules' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int ModifyAnalyticsModules(const char *soap_endpoint_url, const char *soap_action, _tan__ModifyAnalyticsModules *tan__ModifyAnalyticsModules, _tan__ModifyAnalyticsModulesResponse &tan__ModifyAnalyticsModulesResponse) { return this->send_ModifyAnalyticsModules(soap_endpoint_url, soap_action, tan__ModifyAnalyticsModules) || this->recv_ModifyAnalyticsModules(tan__ModifyAnalyticsModulesResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_ModifyAnalyticsModules' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_ModifyAnalyticsModules(const char *soap_endpoint_url, const char *soap_action, _tan__ModifyAnalyticsModules *tan__ModifyAnalyticsModules);
        /// Web service asynchronous operation 'recv_ModifyAnalyticsModules' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_ModifyAnalyticsModules(_tan__ModifyAnalyticsModulesResponse &tan__ModifyAnalyticsModulesResponse);
        //
        /// Web service synchronous operation 'GetSupportedMetadata' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetSupportedMetadata(_tan__GetSupportedMetadata *tan__GetSupportedMetadata, _tan__GetSupportedMetadataResponse &tan__GetSupportedMetadataResponse) { return this->GetSupportedMetadata(NULL, NULL, tan__GetSupportedMetadata, tan__GetSupportedMetadataResponse); }
        /// Web service synchronous operation 'GetSupportedMetadata' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetSupportedMetadata(const char *soap_endpoint_url, const char *soap_action, _tan__GetSupportedMetadata *tan__GetSupportedMetadata, _tan__GetSupportedMetadataResponse &tan__GetSupportedMetadataResponse) { return this->send_GetSupportedMetadata(soap_endpoint_url, soap_action, tan__GetSupportedMetadata) || this->recv_GetSupportedMetadata(tan__GetSupportedMetadataResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetSupportedMetadata' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetSupportedMetadata(const char *soap_endpoint_url, const char *soap_action, _tan__GetSupportedMetadata *tan__GetSupportedMetadata);
        /// Web service asynchronous operation 'recv_GetSupportedMetadata' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetSupportedMetadata(_tan__GetSupportedMetadataResponse &tan__GetSupportedMetadataResponse);
    };
#endif
