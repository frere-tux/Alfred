/**
 * Autogenerated by Thrift Compiler (0.9.3)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef AlfredCommunication_H
#define AlfredCommunication_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include "AlCom_types.h"

namespace AlCom {

#ifdef _WIN32
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class AlfredCommunicationIf {
 public:
  virtual ~AlfredCommunicationIf() {}
  virtual void sendSimpleRequest(const SimpleRequest& request) = 0;
  virtual void sendTask(const ComTask& task) = 0;
};

class AlfredCommunicationIfFactory {
 public:
  typedef AlfredCommunicationIf Handler;

  virtual ~AlfredCommunicationIfFactory() {}

  virtual AlfredCommunicationIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(AlfredCommunicationIf* /* handler */) = 0;
};

class AlfredCommunicationIfSingletonFactory : virtual public AlfredCommunicationIfFactory {
 public:
  AlfredCommunicationIfSingletonFactory(const boost::shared_ptr<AlfredCommunicationIf>& iface) : iface_(iface) {}
  virtual ~AlfredCommunicationIfSingletonFactory() {}

  virtual AlfredCommunicationIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(AlfredCommunicationIf* /* handler */) {}

 protected:
  boost::shared_ptr<AlfredCommunicationIf> iface_;
};

class AlfredCommunicationNull : virtual public AlfredCommunicationIf {
 public:
  virtual ~AlfredCommunicationNull() {}
  void sendSimpleRequest(const SimpleRequest& /* request */) {
    return;
  }
  void sendTask(const ComTask& /* task */) {
    return;
  }
};

typedef struct _AlfredCommunication_sendSimpleRequest_args__isset {
  _AlfredCommunication_sendSimpleRequest_args__isset() : request(false) {}
  bool request :1;
} _AlfredCommunication_sendSimpleRequest_args__isset;

class AlfredCommunication_sendSimpleRequest_args {
 public:

  AlfredCommunication_sendSimpleRequest_args(const AlfredCommunication_sendSimpleRequest_args&);
  AlfredCommunication_sendSimpleRequest_args& operator=(const AlfredCommunication_sendSimpleRequest_args&);
  AlfredCommunication_sendSimpleRequest_args() {
  }

  virtual ~AlfredCommunication_sendSimpleRequest_args() throw();
  SimpleRequest request;

  _AlfredCommunication_sendSimpleRequest_args__isset __isset;

  void __set_request(const SimpleRequest& val);

  bool operator == (const AlfredCommunication_sendSimpleRequest_args & rhs) const
  {
    if (!(request == rhs.request))
      return false;
    return true;
  }
  bool operator != (const AlfredCommunication_sendSimpleRequest_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const AlfredCommunication_sendSimpleRequest_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class AlfredCommunication_sendSimpleRequest_pargs {
 public:


  virtual ~AlfredCommunication_sendSimpleRequest_pargs() throw();
  const SimpleRequest* request;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _AlfredCommunication_sendTask_args__isset {
  _AlfredCommunication_sendTask_args__isset() : task(false) {}
  bool task :1;
} _AlfredCommunication_sendTask_args__isset;

class AlfredCommunication_sendTask_args {
 public:

  AlfredCommunication_sendTask_args(const AlfredCommunication_sendTask_args&);
  AlfredCommunication_sendTask_args& operator=(const AlfredCommunication_sendTask_args&);
  AlfredCommunication_sendTask_args() {
  }

  virtual ~AlfredCommunication_sendTask_args() throw();
  ComTask task;

  _AlfredCommunication_sendTask_args__isset __isset;

  void __set_task(const ComTask& val);

  bool operator == (const AlfredCommunication_sendTask_args & rhs) const
  {
    if (!(task == rhs.task))
      return false;
    return true;
  }
  bool operator != (const AlfredCommunication_sendTask_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const AlfredCommunication_sendTask_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class AlfredCommunication_sendTask_pargs {
 public:


  virtual ~AlfredCommunication_sendTask_pargs() throw();
  const ComTask* task;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

class AlfredCommunicationClient : virtual public AlfredCommunicationIf {
 public:
  AlfredCommunicationClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  AlfredCommunicationClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void sendSimpleRequest(const SimpleRequest& request);
  void send_sendSimpleRequest(const SimpleRequest& request);
  void sendTask(const ComTask& task);
  void send_sendTask(const ComTask& task);
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class AlfredCommunicationProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  boost::shared_ptr<AlfredCommunicationIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (AlfredCommunicationProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_sendSimpleRequest(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_sendTask(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  AlfredCommunicationProcessor(boost::shared_ptr<AlfredCommunicationIf> iface) :
    iface_(iface) {
    processMap_["sendSimpleRequest"] = &AlfredCommunicationProcessor::process_sendSimpleRequest;
    processMap_["sendTask"] = &AlfredCommunicationProcessor::process_sendTask;
  }

  virtual ~AlfredCommunicationProcessor() {}
};

class AlfredCommunicationProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  AlfredCommunicationProcessorFactory(const ::boost::shared_ptr< AlfredCommunicationIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::boost::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::boost::shared_ptr< AlfredCommunicationIfFactory > handlerFactory_;
};

class AlfredCommunicationMultiface : virtual public AlfredCommunicationIf {
 public:
  AlfredCommunicationMultiface(std::vector<boost::shared_ptr<AlfredCommunicationIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~AlfredCommunicationMultiface() {}
 protected:
  std::vector<boost::shared_ptr<AlfredCommunicationIf> > ifaces_;
  AlfredCommunicationMultiface() {}
  void add(boost::shared_ptr<AlfredCommunicationIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void sendSimpleRequest(const SimpleRequest& request) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->sendSimpleRequest(request);
    }
    ifaces_[i]->sendSimpleRequest(request);
  }

  void sendTask(const ComTask& task) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->sendTask(task);
    }
    ifaces_[i]->sendTask(task);
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class AlfredCommunicationConcurrentClient : virtual public AlfredCommunicationIf {
 public:
  AlfredCommunicationConcurrentClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  AlfredCommunicationConcurrentClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void sendSimpleRequest(const SimpleRequest& request);
  void send_sendSimpleRequest(const SimpleRequest& request);
  void sendTask(const ComTask& task);
  void send_sendTask(const ComTask& task);
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
  ::apache::thrift::async::TConcurrentClientSyncInfo sync_;
};

#ifdef _WIN32
  #pragma warning( pop )
#endif

} // namespace

#endif
