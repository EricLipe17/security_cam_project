/* soapAnalyticsDeviceBindingProxy.h
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

#ifndef soapAnalyticsDeviceBindingProxy_H
#define soapAnalyticsDeviceBindingProxy_H
#include "soapH.h"

    class SOAP_CMAC AnalyticsDeviceBindingProxy {
      public:
        /// Context to manage proxy IO and data
        struct soap *soap;
        /// flag indicating that this context is owned by this proxy and should be deleted by the destructor
        bool soap_own;
        /// Endpoint URL of service 'AnalyticsDeviceBindingProxy' (change as needed)
        const char *soap_endpoint;
        /// Variables globally declared in onvif.h, if any
        /// Construct a proxy with new managing context
        AnalyticsDeviceBindingProxy();
        /// Copy constructor
        AnalyticsDeviceBindingProxy(const AnalyticsDeviceBindingProxy& rhs);
        /// Construct proxy given a shared managing context
        AnalyticsDeviceBindingProxy(struct soap*);
        /// Construct proxy given a shared managing context and endpoint URL
        AnalyticsDeviceBindingProxy(struct soap*, const char *soap_endpoint_url);
        /// Constructor taking an endpoint URL
        AnalyticsDeviceBindingProxy(const char *soap_endpoint_url);
        /// Constructor taking input and output mode flags for the new managing context
        AnalyticsDeviceBindingProxy(soap_mode iomode);
        /// Constructor taking endpoint URL and input and output mode flags for the new managing context
        AnalyticsDeviceBindingProxy(const char *soap_endpoint_url, soap_mode iomode);
        /// Constructor taking input and output mode flags for the new managing context
        AnalyticsDeviceBindingProxy(soap_mode imode, soap_mode omode);
        /// Destructor deletes deserialized data and its managing context, when the context was allocated by the constructor
        virtual ~AnalyticsDeviceBindingProxy();
        /// Initializer used by constructors
        virtual void AnalyticsDeviceBindingProxy_init(soap_mode imode, soap_mode omode);
        /// Return a copy that has a new managing context with the same engine state
        virtual AnalyticsDeviceBindingProxy *copy();
        /// Copy assignment
        AnalyticsDeviceBindingProxy& operator=(const AnalyticsDeviceBindingProxy&);
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
        /// Web service synchronous operation 'GetServiceCapabilities' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetServiceCapabilities(_tad__GetServiceCapabilities *tad__GetServiceCapabilities, _tad__GetServiceCapabilitiesResponse &tad__GetServiceCapabilitiesResponse) { return this->GetServiceCapabilities(NULL, NULL, tad__GetServiceCapabilities, tad__GetServiceCapabilitiesResponse); }
        /// Web service synchronous operation 'GetServiceCapabilities' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetServiceCapabilities(const char *soap_endpoint_url, const char *soap_action, _tad__GetServiceCapabilities *tad__GetServiceCapabilities, _tad__GetServiceCapabilitiesResponse &tad__GetServiceCapabilitiesResponse) { return this->send_GetServiceCapabilities(soap_endpoint_url, soap_action, tad__GetServiceCapabilities) || this->recv_GetServiceCapabilities(tad__GetServiceCapabilitiesResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetServiceCapabilities' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetServiceCapabilities(const char *soap_endpoint_url, const char *soap_action, _tad__GetServiceCapabilities *tad__GetServiceCapabilities);
        /// Web service asynchronous operation 'recv_GetServiceCapabilities' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetServiceCapabilities(_tad__GetServiceCapabilitiesResponse &tad__GetServiceCapabilitiesResponse);
        //
        /// Web service synchronous operation 'DeleteAnalyticsEngineControl' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int DeleteAnalyticsEngineControl(_tad__DeleteAnalyticsEngineControl *tad__DeleteAnalyticsEngineControl, _tad__DeleteAnalyticsEngineControlResponse &tad__DeleteAnalyticsEngineControlResponse) { return this->DeleteAnalyticsEngineControl(NULL, NULL, tad__DeleteAnalyticsEngineControl, tad__DeleteAnalyticsEngineControlResponse); }
        /// Web service synchronous operation 'DeleteAnalyticsEngineControl' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int DeleteAnalyticsEngineControl(const char *soap_endpoint_url, const char *soap_action, _tad__DeleteAnalyticsEngineControl *tad__DeleteAnalyticsEngineControl, _tad__DeleteAnalyticsEngineControlResponse &tad__DeleteAnalyticsEngineControlResponse) { return this->send_DeleteAnalyticsEngineControl(soap_endpoint_url, soap_action, tad__DeleteAnalyticsEngineControl) || this->recv_DeleteAnalyticsEngineControl(tad__DeleteAnalyticsEngineControlResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_DeleteAnalyticsEngineControl' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_DeleteAnalyticsEngineControl(const char *soap_endpoint_url, const char *soap_action, _tad__DeleteAnalyticsEngineControl *tad__DeleteAnalyticsEngineControl);
        /// Web service asynchronous operation 'recv_DeleteAnalyticsEngineControl' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_DeleteAnalyticsEngineControl(_tad__DeleteAnalyticsEngineControlResponse &tad__DeleteAnalyticsEngineControlResponse);
        //
        /// Web service synchronous operation 'CreateAnalyticsEngineControl' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int CreateAnalyticsEngineControl(_tad__CreateAnalyticsEngineControl *tad__CreateAnalyticsEngineControl, _tad__CreateAnalyticsEngineControlResponse &tad__CreateAnalyticsEngineControlResponse) { return this->CreateAnalyticsEngineControl(NULL, NULL, tad__CreateAnalyticsEngineControl, tad__CreateAnalyticsEngineControlResponse); }
        /// Web service synchronous operation 'CreateAnalyticsEngineControl' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int CreateAnalyticsEngineControl(const char *soap_endpoint_url, const char *soap_action, _tad__CreateAnalyticsEngineControl *tad__CreateAnalyticsEngineControl, _tad__CreateAnalyticsEngineControlResponse &tad__CreateAnalyticsEngineControlResponse) { return this->send_CreateAnalyticsEngineControl(soap_endpoint_url, soap_action, tad__CreateAnalyticsEngineControl) || this->recv_CreateAnalyticsEngineControl(tad__CreateAnalyticsEngineControlResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_CreateAnalyticsEngineControl' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_CreateAnalyticsEngineControl(const char *soap_endpoint_url, const char *soap_action, _tad__CreateAnalyticsEngineControl *tad__CreateAnalyticsEngineControl);
        /// Web service asynchronous operation 'recv_CreateAnalyticsEngineControl' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_CreateAnalyticsEngineControl(_tad__CreateAnalyticsEngineControlResponse &tad__CreateAnalyticsEngineControlResponse);
        //
        /// Web service synchronous operation 'SetAnalyticsEngineControl' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int SetAnalyticsEngineControl(_tad__SetAnalyticsEngineControl *tad__SetAnalyticsEngineControl, _tad__SetAnalyticsEngineControlResponse &tad__SetAnalyticsEngineControlResponse) { return this->SetAnalyticsEngineControl(NULL, NULL, tad__SetAnalyticsEngineControl, tad__SetAnalyticsEngineControlResponse); }
        /// Web service synchronous operation 'SetAnalyticsEngineControl' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int SetAnalyticsEngineControl(const char *soap_endpoint_url, const char *soap_action, _tad__SetAnalyticsEngineControl *tad__SetAnalyticsEngineControl, _tad__SetAnalyticsEngineControlResponse &tad__SetAnalyticsEngineControlResponse) { return this->send_SetAnalyticsEngineControl(soap_endpoint_url, soap_action, tad__SetAnalyticsEngineControl) || this->recv_SetAnalyticsEngineControl(tad__SetAnalyticsEngineControlResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_SetAnalyticsEngineControl' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_SetAnalyticsEngineControl(const char *soap_endpoint_url, const char *soap_action, _tad__SetAnalyticsEngineControl *tad__SetAnalyticsEngineControl);
        /// Web service asynchronous operation 'recv_SetAnalyticsEngineControl' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_SetAnalyticsEngineControl(_tad__SetAnalyticsEngineControlResponse &tad__SetAnalyticsEngineControlResponse);
        //
        /// Web service synchronous operation 'GetAnalyticsEngineControl' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetAnalyticsEngineControl(_tad__GetAnalyticsEngineControl *tad__GetAnalyticsEngineControl, _tad__GetAnalyticsEngineControlResponse &tad__GetAnalyticsEngineControlResponse) { return this->GetAnalyticsEngineControl(NULL, NULL, tad__GetAnalyticsEngineControl, tad__GetAnalyticsEngineControlResponse); }
        /// Web service synchronous operation 'GetAnalyticsEngineControl' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetAnalyticsEngineControl(const char *soap_endpoint_url, const char *soap_action, _tad__GetAnalyticsEngineControl *tad__GetAnalyticsEngineControl, _tad__GetAnalyticsEngineControlResponse &tad__GetAnalyticsEngineControlResponse) { return this->send_GetAnalyticsEngineControl(soap_endpoint_url, soap_action, tad__GetAnalyticsEngineControl) || this->recv_GetAnalyticsEngineControl(tad__GetAnalyticsEngineControlResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetAnalyticsEngineControl' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetAnalyticsEngineControl(const char *soap_endpoint_url, const char *soap_action, _tad__GetAnalyticsEngineControl *tad__GetAnalyticsEngineControl);
        /// Web service asynchronous operation 'recv_GetAnalyticsEngineControl' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetAnalyticsEngineControl(_tad__GetAnalyticsEngineControlResponse &tad__GetAnalyticsEngineControlResponse);
        //
        /// Web service synchronous operation 'GetAnalyticsEngineControls' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetAnalyticsEngineControls(_tad__GetAnalyticsEngineControls *tad__GetAnalyticsEngineControls, _tad__GetAnalyticsEngineControlsResponse &tad__GetAnalyticsEngineControlsResponse) { return this->GetAnalyticsEngineControls(NULL, NULL, tad__GetAnalyticsEngineControls, tad__GetAnalyticsEngineControlsResponse); }
        /// Web service synchronous operation 'GetAnalyticsEngineControls' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetAnalyticsEngineControls(const char *soap_endpoint_url, const char *soap_action, _tad__GetAnalyticsEngineControls *tad__GetAnalyticsEngineControls, _tad__GetAnalyticsEngineControlsResponse &tad__GetAnalyticsEngineControlsResponse) { return this->send_GetAnalyticsEngineControls(soap_endpoint_url, soap_action, tad__GetAnalyticsEngineControls) || this->recv_GetAnalyticsEngineControls(tad__GetAnalyticsEngineControlsResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetAnalyticsEngineControls' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetAnalyticsEngineControls(const char *soap_endpoint_url, const char *soap_action, _tad__GetAnalyticsEngineControls *tad__GetAnalyticsEngineControls);
        /// Web service asynchronous operation 'recv_GetAnalyticsEngineControls' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetAnalyticsEngineControls(_tad__GetAnalyticsEngineControlsResponse &tad__GetAnalyticsEngineControlsResponse);
        //
        /// Web service synchronous operation 'GetAnalyticsEngine' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetAnalyticsEngine(_tad__GetAnalyticsEngine *tad__GetAnalyticsEngine, _tad__GetAnalyticsEngineResponse &tad__GetAnalyticsEngineResponse) { return this->GetAnalyticsEngine(NULL, NULL, tad__GetAnalyticsEngine, tad__GetAnalyticsEngineResponse); }
        /// Web service synchronous operation 'GetAnalyticsEngine' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetAnalyticsEngine(const char *soap_endpoint_url, const char *soap_action, _tad__GetAnalyticsEngine *tad__GetAnalyticsEngine, _tad__GetAnalyticsEngineResponse &tad__GetAnalyticsEngineResponse) { return this->send_GetAnalyticsEngine(soap_endpoint_url, soap_action, tad__GetAnalyticsEngine) || this->recv_GetAnalyticsEngine(tad__GetAnalyticsEngineResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetAnalyticsEngine' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetAnalyticsEngine(const char *soap_endpoint_url, const char *soap_action, _tad__GetAnalyticsEngine *tad__GetAnalyticsEngine);
        /// Web service asynchronous operation 'recv_GetAnalyticsEngine' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetAnalyticsEngine(_tad__GetAnalyticsEngineResponse &tad__GetAnalyticsEngineResponse);
        //
        /// Web service synchronous operation 'GetAnalyticsEngines' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetAnalyticsEngines(_tad__GetAnalyticsEngines *tad__GetAnalyticsEngines, _tad__GetAnalyticsEnginesResponse &tad__GetAnalyticsEnginesResponse) { return this->GetAnalyticsEngines(NULL, NULL, tad__GetAnalyticsEngines, tad__GetAnalyticsEnginesResponse); }
        /// Web service synchronous operation 'GetAnalyticsEngines' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetAnalyticsEngines(const char *soap_endpoint_url, const char *soap_action, _tad__GetAnalyticsEngines *tad__GetAnalyticsEngines, _tad__GetAnalyticsEnginesResponse &tad__GetAnalyticsEnginesResponse) { return this->send_GetAnalyticsEngines(soap_endpoint_url, soap_action, tad__GetAnalyticsEngines) || this->recv_GetAnalyticsEngines(tad__GetAnalyticsEnginesResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetAnalyticsEngines' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetAnalyticsEngines(const char *soap_endpoint_url, const char *soap_action, _tad__GetAnalyticsEngines *tad__GetAnalyticsEngines);
        /// Web service asynchronous operation 'recv_GetAnalyticsEngines' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetAnalyticsEngines(_tad__GetAnalyticsEnginesResponse &tad__GetAnalyticsEnginesResponse);
        //
        /// Web service synchronous operation 'SetVideoAnalyticsConfiguration' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int SetVideoAnalyticsConfiguration(_tad__SetVideoAnalyticsConfiguration *tad__SetVideoAnalyticsConfiguration, _tad__SetVideoAnalyticsConfigurationResponse &tad__SetVideoAnalyticsConfigurationResponse) { return this->SetVideoAnalyticsConfiguration(NULL, NULL, tad__SetVideoAnalyticsConfiguration, tad__SetVideoAnalyticsConfigurationResponse); }
        /// Web service synchronous operation 'SetVideoAnalyticsConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int SetVideoAnalyticsConfiguration(const char *soap_endpoint_url, const char *soap_action, _tad__SetVideoAnalyticsConfiguration *tad__SetVideoAnalyticsConfiguration, _tad__SetVideoAnalyticsConfigurationResponse &tad__SetVideoAnalyticsConfigurationResponse) { return this->send_SetVideoAnalyticsConfiguration(soap_endpoint_url, soap_action, tad__SetVideoAnalyticsConfiguration) || this->recv_SetVideoAnalyticsConfiguration(tad__SetVideoAnalyticsConfigurationResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_SetVideoAnalyticsConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_SetVideoAnalyticsConfiguration(const char *soap_endpoint_url, const char *soap_action, _tad__SetVideoAnalyticsConfiguration *tad__SetVideoAnalyticsConfiguration);
        /// Web service asynchronous operation 'recv_SetVideoAnalyticsConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_SetVideoAnalyticsConfiguration(_tad__SetVideoAnalyticsConfigurationResponse &tad__SetVideoAnalyticsConfigurationResponse);
        //
        /// Web service synchronous operation 'SetAnalyticsEngineInput' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int SetAnalyticsEngineInput(_tad__SetAnalyticsEngineInput *tad__SetAnalyticsEngineInput, _tad__SetAnalyticsEngineInputResponse &tad__SetAnalyticsEngineInputResponse) { return this->SetAnalyticsEngineInput(NULL, NULL, tad__SetAnalyticsEngineInput, tad__SetAnalyticsEngineInputResponse); }
        /// Web service synchronous operation 'SetAnalyticsEngineInput' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int SetAnalyticsEngineInput(const char *soap_endpoint_url, const char *soap_action, _tad__SetAnalyticsEngineInput *tad__SetAnalyticsEngineInput, _tad__SetAnalyticsEngineInputResponse &tad__SetAnalyticsEngineInputResponse) { return this->send_SetAnalyticsEngineInput(soap_endpoint_url, soap_action, tad__SetAnalyticsEngineInput) || this->recv_SetAnalyticsEngineInput(tad__SetAnalyticsEngineInputResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_SetAnalyticsEngineInput' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_SetAnalyticsEngineInput(const char *soap_endpoint_url, const char *soap_action, _tad__SetAnalyticsEngineInput *tad__SetAnalyticsEngineInput);
        /// Web service asynchronous operation 'recv_SetAnalyticsEngineInput' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_SetAnalyticsEngineInput(_tad__SetAnalyticsEngineInputResponse &tad__SetAnalyticsEngineInputResponse);
        //
        /// Web service synchronous operation 'GetAnalyticsEngineInput' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetAnalyticsEngineInput(_tad__GetAnalyticsEngineInput *tad__GetAnalyticsEngineInput, _tad__GetAnalyticsEngineInputResponse &tad__GetAnalyticsEngineInputResponse) { return this->GetAnalyticsEngineInput(NULL, NULL, tad__GetAnalyticsEngineInput, tad__GetAnalyticsEngineInputResponse); }
        /// Web service synchronous operation 'GetAnalyticsEngineInput' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetAnalyticsEngineInput(const char *soap_endpoint_url, const char *soap_action, _tad__GetAnalyticsEngineInput *tad__GetAnalyticsEngineInput, _tad__GetAnalyticsEngineInputResponse &tad__GetAnalyticsEngineInputResponse) { return this->send_GetAnalyticsEngineInput(soap_endpoint_url, soap_action, tad__GetAnalyticsEngineInput) || this->recv_GetAnalyticsEngineInput(tad__GetAnalyticsEngineInputResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetAnalyticsEngineInput' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetAnalyticsEngineInput(const char *soap_endpoint_url, const char *soap_action, _tad__GetAnalyticsEngineInput *tad__GetAnalyticsEngineInput);
        /// Web service asynchronous operation 'recv_GetAnalyticsEngineInput' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetAnalyticsEngineInput(_tad__GetAnalyticsEngineInputResponse &tad__GetAnalyticsEngineInputResponse);
        //
        /// Web service synchronous operation 'GetAnalyticsEngineInputs' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetAnalyticsEngineInputs(_tad__GetAnalyticsEngineInputs *tad__GetAnalyticsEngineInputs, _tad__GetAnalyticsEngineInputsResponse &tad__GetAnalyticsEngineInputsResponse) { return this->GetAnalyticsEngineInputs(NULL, NULL, tad__GetAnalyticsEngineInputs, tad__GetAnalyticsEngineInputsResponse); }
        /// Web service synchronous operation 'GetAnalyticsEngineInputs' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetAnalyticsEngineInputs(const char *soap_endpoint_url, const char *soap_action, _tad__GetAnalyticsEngineInputs *tad__GetAnalyticsEngineInputs, _tad__GetAnalyticsEngineInputsResponse &tad__GetAnalyticsEngineInputsResponse) { return this->send_GetAnalyticsEngineInputs(soap_endpoint_url, soap_action, tad__GetAnalyticsEngineInputs) || this->recv_GetAnalyticsEngineInputs(tad__GetAnalyticsEngineInputsResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetAnalyticsEngineInputs' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetAnalyticsEngineInputs(const char *soap_endpoint_url, const char *soap_action, _tad__GetAnalyticsEngineInputs *tad__GetAnalyticsEngineInputs);
        /// Web service asynchronous operation 'recv_GetAnalyticsEngineInputs' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetAnalyticsEngineInputs(_tad__GetAnalyticsEngineInputsResponse &tad__GetAnalyticsEngineInputsResponse);
        //
        /// Web service synchronous operation 'GetAnalyticsDeviceStreamUri' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetAnalyticsDeviceStreamUri(_tad__GetAnalyticsDeviceStreamUri *tad__GetAnalyticsDeviceStreamUri, _tad__GetAnalyticsDeviceStreamUriResponse &tad__GetAnalyticsDeviceStreamUriResponse) { return this->GetAnalyticsDeviceStreamUri(NULL, NULL, tad__GetAnalyticsDeviceStreamUri, tad__GetAnalyticsDeviceStreamUriResponse); }
        /// Web service synchronous operation 'GetAnalyticsDeviceStreamUri' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetAnalyticsDeviceStreamUri(const char *soap_endpoint_url, const char *soap_action, _tad__GetAnalyticsDeviceStreamUri *tad__GetAnalyticsDeviceStreamUri, _tad__GetAnalyticsDeviceStreamUriResponse &tad__GetAnalyticsDeviceStreamUriResponse) { return this->send_GetAnalyticsDeviceStreamUri(soap_endpoint_url, soap_action, tad__GetAnalyticsDeviceStreamUri) || this->recv_GetAnalyticsDeviceStreamUri(tad__GetAnalyticsDeviceStreamUriResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetAnalyticsDeviceStreamUri' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetAnalyticsDeviceStreamUri(const char *soap_endpoint_url, const char *soap_action, _tad__GetAnalyticsDeviceStreamUri *tad__GetAnalyticsDeviceStreamUri);
        /// Web service asynchronous operation 'recv_GetAnalyticsDeviceStreamUri' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetAnalyticsDeviceStreamUri(_tad__GetAnalyticsDeviceStreamUriResponse &tad__GetAnalyticsDeviceStreamUriResponse);
        //
        /// Web service synchronous operation 'GetVideoAnalyticsConfiguration' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetVideoAnalyticsConfiguration(_tad__GetVideoAnalyticsConfiguration *tad__GetVideoAnalyticsConfiguration, _tad__GetVideoAnalyticsConfigurationResponse &tad__GetVideoAnalyticsConfigurationResponse) { return this->GetVideoAnalyticsConfiguration(NULL, NULL, tad__GetVideoAnalyticsConfiguration, tad__GetVideoAnalyticsConfigurationResponse); }
        /// Web service synchronous operation 'GetVideoAnalyticsConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetVideoAnalyticsConfiguration(const char *soap_endpoint_url, const char *soap_action, _tad__GetVideoAnalyticsConfiguration *tad__GetVideoAnalyticsConfiguration, _tad__GetVideoAnalyticsConfigurationResponse &tad__GetVideoAnalyticsConfigurationResponse) { return this->send_GetVideoAnalyticsConfiguration(soap_endpoint_url, soap_action, tad__GetVideoAnalyticsConfiguration) || this->recv_GetVideoAnalyticsConfiguration(tad__GetVideoAnalyticsConfigurationResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetVideoAnalyticsConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetVideoAnalyticsConfiguration(const char *soap_endpoint_url, const char *soap_action, _tad__GetVideoAnalyticsConfiguration *tad__GetVideoAnalyticsConfiguration);
        /// Web service asynchronous operation 'recv_GetVideoAnalyticsConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetVideoAnalyticsConfiguration(_tad__GetVideoAnalyticsConfigurationResponse &tad__GetVideoAnalyticsConfigurationResponse);
        //
        /// Web service synchronous operation 'CreateAnalyticsEngineInputs' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int CreateAnalyticsEngineInputs(_tad__CreateAnalyticsEngineInputs *tad__CreateAnalyticsEngineInputs, _tad__CreateAnalyticsEngineInputsResponse &tad__CreateAnalyticsEngineInputsResponse) { return this->CreateAnalyticsEngineInputs(NULL, NULL, tad__CreateAnalyticsEngineInputs, tad__CreateAnalyticsEngineInputsResponse); }
        /// Web service synchronous operation 'CreateAnalyticsEngineInputs' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int CreateAnalyticsEngineInputs(const char *soap_endpoint_url, const char *soap_action, _tad__CreateAnalyticsEngineInputs *tad__CreateAnalyticsEngineInputs, _tad__CreateAnalyticsEngineInputsResponse &tad__CreateAnalyticsEngineInputsResponse) { return this->send_CreateAnalyticsEngineInputs(soap_endpoint_url, soap_action, tad__CreateAnalyticsEngineInputs) || this->recv_CreateAnalyticsEngineInputs(tad__CreateAnalyticsEngineInputsResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_CreateAnalyticsEngineInputs' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_CreateAnalyticsEngineInputs(const char *soap_endpoint_url, const char *soap_action, _tad__CreateAnalyticsEngineInputs *tad__CreateAnalyticsEngineInputs);
        /// Web service asynchronous operation 'recv_CreateAnalyticsEngineInputs' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_CreateAnalyticsEngineInputs(_tad__CreateAnalyticsEngineInputsResponse &tad__CreateAnalyticsEngineInputsResponse);
        //
        /// Web service synchronous operation 'DeleteAnalyticsEngineInputs' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int DeleteAnalyticsEngineInputs(_tad__DeleteAnalyticsEngineInputs *tad__DeleteAnalyticsEngineInputs, _tad__DeleteAnalyticsEngineInputsResponse &tad__DeleteAnalyticsEngineInputsResponse) { return this->DeleteAnalyticsEngineInputs(NULL, NULL, tad__DeleteAnalyticsEngineInputs, tad__DeleteAnalyticsEngineInputsResponse); }
        /// Web service synchronous operation 'DeleteAnalyticsEngineInputs' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int DeleteAnalyticsEngineInputs(const char *soap_endpoint_url, const char *soap_action, _tad__DeleteAnalyticsEngineInputs *tad__DeleteAnalyticsEngineInputs, _tad__DeleteAnalyticsEngineInputsResponse &tad__DeleteAnalyticsEngineInputsResponse) { return this->send_DeleteAnalyticsEngineInputs(soap_endpoint_url, soap_action, tad__DeleteAnalyticsEngineInputs) || this->recv_DeleteAnalyticsEngineInputs(tad__DeleteAnalyticsEngineInputsResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_DeleteAnalyticsEngineInputs' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_DeleteAnalyticsEngineInputs(const char *soap_endpoint_url, const char *soap_action, _tad__DeleteAnalyticsEngineInputs *tad__DeleteAnalyticsEngineInputs);
        /// Web service asynchronous operation 'recv_DeleteAnalyticsEngineInputs' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_DeleteAnalyticsEngineInputs(_tad__DeleteAnalyticsEngineInputsResponse &tad__DeleteAnalyticsEngineInputsResponse);
        //
        /// Web service synchronous operation 'GetAnalyticsState' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetAnalyticsState(_tad__GetAnalyticsState *tad__GetAnalyticsState, _tad__GetAnalyticsStateResponse &tad__GetAnalyticsStateResponse) { return this->GetAnalyticsState(NULL, NULL, tad__GetAnalyticsState, tad__GetAnalyticsStateResponse); }
        /// Web service synchronous operation 'GetAnalyticsState' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetAnalyticsState(const char *soap_endpoint_url, const char *soap_action, _tad__GetAnalyticsState *tad__GetAnalyticsState, _tad__GetAnalyticsStateResponse &tad__GetAnalyticsStateResponse) { return this->send_GetAnalyticsState(soap_endpoint_url, soap_action, tad__GetAnalyticsState) || this->recv_GetAnalyticsState(tad__GetAnalyticsStateResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetAnalyticsState' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetAnalyticsState(const char *soap_endpoint_url, const char *soap_action, _tad__GetAnalyticsState *tad__GetAnalyticsState);
        /// Web service asynchronous operation 'recv_GetAnalyticsState' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetAnalyticsState(_tad__GetAnalyticsStateResponse &tad__GetAnalyticsStateResponse);
    };
#endif
