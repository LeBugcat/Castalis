//
// Generated file, do not edit! Created by nedtool 5.6 from node/communication/routing/RoutingPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include "RoutingPacket_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp


// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

NetMacInfoExchange_type::NetMacInfoExchange_type()
{
    this->RSSI = 0;
    this->LQI = 0;
    this->nextHop = 0;
    this->lastHop = 0;
}

void __doPacking(omnetpp::cCommBuffer *b, const NetMacInfoExchange_type& a)
{
    doParsimPacking(b,a.RSSI);
    doParsimPacking(b,a.LQI);
    doParsimPacking(b,a.nextHop);
    doParsimPacking(b,a.lastHop);
}

void __doUnpacking(omnetpp::cCommBuffer *b, NetMacInfoExchange_type& a)
{
    doParsimUnpacking(b,a.RSSI);
    doParsimUnpacking(b,a.LQI);
    doParsimUnpacking(b,a.nextHop);
    doParsimUnpacking(b,a.lastHop);
}

class NetMacInfoExchange_typeDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    NetMacInfoExchange_typeDescriptor();
    virtual ~NetMacInfoExchange_typeDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(NetMacInfoExchange_typeDescriptor)

NetMacInfoExchange_typeDescriptor::NetMacInfoExchange_typeDescriptor() : omnetpp::cClassDescriptor("NetMacInfoExchange_type", "")
{
    propertynames = nullptr;
}

NetMacInfoExchange_typeDescriptor::~NetMacInfoExchange_typeDescriptor()
{
    delete[] propertynames;
}

bool NetMacInfoExchange_typeDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<NetMacInfoExchange_type *>(obj)!=nullptr;
}

const char **NetMacInfoExchange_typeDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *NetMacInfoExchange_typeDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int NetMacInfoExchange_typeDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount() : 4;
}

unsigned int NetMacInfoExchange_typeDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *NetMacInfoExchange_typeDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "RSSI",
        "LQI",
        "nextHop",
        "lastHop",
    };
    return (field>=0 && field<4) ? fieldNames[field] : nullptr;
}

int NetMacInfoExchange_typeDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='R' && strcmp(fieldName, "RSSI")==0) return base+0;
    if (fieldName[0]=='L' && strcmp(fieldName, "LQI")==0) return base+1;
    if (fieldName[0]=='n' && strcmp(fieldName, "nextHop")==0) return base+2;
    if (fieldName[0]=='l' && strcmp(fieldName, "lastHop")==0) return base+3;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *NetMacInfoExchange_typeDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "double",
        "double",
        "int",
        "int",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : nullptr;
}

const char **NetMacInfoExchange_typeDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *NetMacInfoExchange_typeDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int NetMacInfoExchange_typeDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    NetMacInfoExchange_type *pp = (NetMacInfoExchange_type *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *NetMacInfoExchange_typeDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    NetMacInfoExchange_type *pp = (NetMacInfoExchange_type *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string NetMacInfoExchange_typeDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    NetMacInfoExchange_type *pp = (NetMacInfoExchange_type *)object; (void)pp;
    switch (field) {
        case 0: return double2string(pp->RSSI);
        case 1: return double2string(pp->LQI);
        case 2: return long2string(pp->nextHop);
        case 3: return long2string(pp->lastHop);
        default: return "";
    }
}

bool NetMacInfoExchange_typeDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    NetMacInfoExchange_type *pp = (NetMacInfoExchange_type *)object; (void)pp;
    switch (field) {
        case 0: pp->RSSI = string2double(value); return true;
        case 1: pp->LQI = string2double(value); return true;
        case 2: pp->nextHop = string2long(value); return true;
        case 3: pp->lastHop = string2long(value); return true;
        default: return false;
    }
}

const char *NetMacInfoExchange_typeDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *NetMacInfoExchange_typeDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    NetMacInfoExchange_type *pp = (NetMacInfoExchange_type *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(RoutingPacket)

RoutingPacket::RoutingPacket(const char *name, short kind) : ::omnetpp::cPacket(name,kind)
{
    this->TTL = 10000;
    this->hopCount = 0;
    this->distanceCount = 0;
    this->source = 0;
    this->destination = 0;
    this->sequenceNumber = 0;
}

RoutingPacket::RoutingPacket(const RoutingPacket& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

RoutingPacket::~RoutingPacket()
{
}

RoutingPacket& RoutingPacket::operator=(const RoutingPacket& other)
{
    if (this==&other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void RoutingPacket::copy(const RoutingPacket& other)
{
    this->netMacInfoExchange = other.netMacInfoExchange;
    this->TTL = other.TTL;
    this->hopCount = other.hopCount;
    this->distanceCount = other.distanceCount;
    this->source = other.source;
    this->destination = other.destination;
    this->sourceAddress = other.sourceAddress;
    this->destinationAddress = other.destinationAddress;
    this->sequenceNumber = other.sequenceNumber;
}

void RoutingPacket::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->netMacInfoExchange);
    doParsimPacking(b,this->TTL);
    doParsimPacking(b,this->hopCount);
    doParsimPacking(b,this->distanceCount);
    doParsimPacking(b,this->source);
    doParsimPacking(b,this->destination);
    doParsimPacking(b,this->sourceAddress);
    doParsimPacking(b,this->destinationAddress);
    doParsimPacking(b,this->sequenceNumber);
}

void RoutingPacket::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->netMacInfoExchange);
    doParsimUnpacking(b,this->TTL);
    doParsimUnpacking(b,this->hopCount);
    doParsimUnpacking(b,this->distanceCount);
    doParsimUnpacking(b,this->source);
    doParsimUnpacking(b,this->destination);
    doParsimUnpacking(b,this->sourceAddress);
    doParsimUnpacking(b,this->destinationAddress);
    doParsimUnpacking(b,this->sequenceNumber);
}

NetMacInfoExchange_type& RoutingPacket::getNetMacInfoExchange()
{
    return this->netMacInfoExchange;
}

void RoutingPacket::setNetMacInfoExchange(const NetMacInfoExchange_type& netMacInfoExchange)
{
    this->netMacInfoExchange = netMacInfoExchange;
}

int RoutingPacket::getTTL() const
{
    return this->TTL;
}

void RoutingPacket::setTTL(int TTL)
{
    this->TTL = TTL;
}

int RoutingPacket::getHopCount() const
{
    return this->hopCount;
}

void RoutingPacket::setHopCount(int hopCount)
{
    this->hopCount = hopCount;
}

double RoutingPacket::getDistanceCount() const
{
    return this->distanceCount;
}

void RoutingPacket::setDistanceCount(double distanceCount)
{
    this->distanceCount = distanceCount;
}

int RoutingPacket::getSource() const
{
    return this->source;
}

void RoutingPacket::setSource(int source)
{
    this->source = source;
}

int RoutingPacket::getDestination() const
{
    return this->destination;
}

void RoutingPacket::setDestination(int destination)
{
    this->destination = destination;
}

const char * RoutingPacket::getSourceAddress() const
{
    return this->sourceAddress.c_str();
}

void RoutingPacket::setSourceAddress(const char * sourceAddress)
{
    this->sourceAddress = sourceAddress;
}

const char * RoutingPacket::getDestinationAddress() const
{
    return this->destinationAddress.c_str();
}

void RoutingPacket::setDestinationAddress(const char * destinationAddress)
{
    this->destinationAddress = destinationAddress;
}

unsigned int RoutingPacket::getSequenceNumber() const
{
    return this->sequenceNumber;
}

void RoutingPacket::setSequenceNumber(unsigned int sequenceNumber)
{
    this->sequenceNumber = sequenceNumber;
}

class RoutingPacketDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    RoutingPacketDescriptor();
    virtual ~RoutingPacketDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(RoutingPacketDescriptor)

RoutingPacketDescriptor::RoutingPacketDescriptor() : omnetpp::cClassDescriptor("RoutingPacket", "omnetpp::cPacket")
{
    propertynames = nullptr;
}

RoutingPacketDescriptor::~RoutingPacketDescriptor()
{
    delete[] propertynames;
}

bool RoutingPacketDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<RoutingPacket *>(obj)!=nullptr;
}

const char **RoutingPacketDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *RoutingPacketDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int RoutingPacketDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 9+basedesc->getFieldCount() : 9;
}

unsigned int RoutingPacketDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<9) ? fieldTypeFlags[field] : 0;
}

const char *RoutingPacketDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "netMacInfoExchange",
        "TTL",
        "hopCount",
        "distanceCount",
        "source",
        "destination",
        "sourceAddress",
        "destinationAddress",
        "sequenceNumber",
    };
    return (field>=0 && field<9) ? fieldNames[field] : nullptr;
}

int RoutingPacketDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='n' && strcmp(fieldName, "netMacInfoExchange")==0) return base+0;
    if (fieldName[0]=='T' && strcmp(fieldName, "TTL")==0) return base+1;
    if (fieldName[0]=='h' && strcmp(fieldName, "hopCount")==0) return base+2;
    if (fieldName[0]=='d' && strcmp(fieldName, "distanceCount")==0) return base+3;
    if (fieldName[0]=='s' && strcmp(fieldName, "source")==0) return base+4;
    if (fieldName[0]=='d' && strcmp(fieldName, "destination")==0) return base+5;
    if (fieldName[0]=='s' && strcmp(fieldName, "sourceAddress")==0) return base+6;
    if (fieldName[0]=='d' && strcmp(fieldName, "destinationAddress")==0) return base+7;
    if (fieldName[0]=='s' && strcmp(fieldName, "sequenceNumber")==0) return base+8;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *RoutingPacketDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "NetMacInfoExchange_type",
        "int",
        "int",
        "double",
        "int",
        "int",
        "string",
        "string",
        "unsigned int",
    };
    return (field>=0 && field<9) ? fieldTypeStrings[field] : nullptr;
}

const char **RoutingPacketDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *RoutingPacketDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int RoutingPacketDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    RoutingPacket *pp = (RoutingPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *RoutingPacketDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    RoutingPacket *pp = (RoutingPacket *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string RoutingPacketDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    RoutingPacket *pp = (RoutingPacket *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getNetMacInfoExchange(); return out.str();}
        case 1: return long2string(pp->getTTL());
        case 2: return long2string(pp->getHopCount());
        case 3: return double2string(pp->getDistanceCount());
        case 4: return long2string(pp->getSource());
        case 5: return long2string(pp->getDestination());
        case 6: return oppstring2string(pp->getSourceAddress());
        case 7: return oppstring2string(pp->getDestinationAddress());
        case 8: return ulong2string(pp->getSequenceNumber());
        default: return "";
    }
}

bool RoutingPacketDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    RoutingPacket *pp = (RoutingPacket *)object; (void)pp;
    switch (field) {
        case 1: pp->setTTL(string2long(value)); return true;
        case 2: pp->setHopCount(string2long(value)); return true;
        case 3: pp->setDistanceCount(string2double(value)); return true;
        case 4: pp->setSource(string2long(value)); return true;
        case 5: pp->setDestination(string2long(value)); return true;
        case 6: pp->setSourceAddress((value)); return true;
        case 7: pp->setDestinationAddress((value)); return true;
        case 8: pp->setSequenceNumber(string2ulong(value)); return true;
        default: return false;
    }
}

const char *RoutingPacketDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 0: return omnetpp::opp_typename(typeid(NetMacInfoExchange_type));
        default: return nullptr;
    };
}

void *RoutingPacketDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    RoutingPacket *pp = (RoutingPacket *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getNetMacInfoExchange()); break;
        default: return nullptr;
    }
}


