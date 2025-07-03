//
// Generated file, do not edit! Created by nedtool 5.6 from node/communication/routing/konstant2019/konstant2019Packet.msg.
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
#include "konstant2019Packet_m.h"

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

EXECUTE_ON_STARTUP(
    omnetpp::cEnum *e = omnetpp::cEnum::find("konstant2019Packet_Type");
    if (!e) omnetpp::enums.getInstance()->add(e = new omnetpp::cEnum("konstant2019Packet_Type"));
    e->insert(GP_ROUTING_CONTROL_PACKET, "GP_ROUTING_CONTROL_PACKET");
    e->insert(GP_ROUTING_DATA_PACKET, "GP_ROUTING_DATA_PACKET");
    e->insert(GP_ROUTING_JOIN_PACKET, "GP_ROUTING_JOIN_PACKET");
    e->insert(GP_ROUTING_ADV_PACKET, "GP_ROUTING_ADV_PACKET");
)

Register_Class(konstant2019Packet)

konstant2019Packet::konstant2019Packet(const char *name, short kind) : ::RoutingPacket(name,kind)
{
    this->konstant2019PacketKind = 0;
    this->sourceId = 0;
    schedule_arraysize = 0;
    this->schedule = 0;
    this->remainingEnergy = 0;
    this->sourceCH = 0;
}

konstant2019Packet::konstant2019Packet(const konstant2019Packet& other) : ::RoutingPacket(other)
{
    schedule_arraysize = 0;
    this->schedule = 0;
    copy(other);
}

konstant2019Packet::~konstant2019Packet()
{
    delete [] this->schedule;
}

konstant2019Packet& konstant2019Packet::operator=(const konstant2019Packet& other)
{
    if (this==&other) return *this;
    ::RoutingPacket::operator=(other);
    copy(other);
    return *this;
}

void konstant2019Packet::copy(const konstant2019Packet& other)
{
    this->konstant2019PacketKind = other.konstant2019PacketKind;
    this->sourceId = other.sourceId;
    delete [] this->schedule;
    this->schedule = (other.schedule_arraysize==0) ? nullptr : new int[other.schedule_arraysize];
    schedule_arraysize = other.schedule_arraysize;
    for (unsigned int i=0; i<schedule_arraysize; i++)
        this->schedule[i] = other.schedule[i];
    this->remainingEnergy = other.remainingEnergy;
    this->sourceCH = other.sourceCH;
    this->isCH = other.isCH;
    this->nextCH = other.nextCH;
    this->trace = other.trace;
}

void konstant2019Packet::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::RoutingPacket::parsimPack(b);
    doParsimPacking(b,this->konstant2019PacketKind);
    doParsimPacking(b,this->sourceId);
    b->pack(schedule_arraysize);
    doParsimArrayPacking(b,this->schedule,schedule_arraysize);
    doParsimPacking(b,this->remainingEnergy);
    doParsimPacking(b,this->sourceCH);
    doParsimPacking(b,this->isCH);
    doParsimPacking(b,this->nextCH);
    doParsimPacking(b,this->trace);
}

void konstant2019Packet::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::RoutingPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->konstant2019PacketKind);
    doParsimUnpacking(b,this->sourceId);
    delete [] this->schedule;
    b->unpack(schedule_arraysize);
    if (schedule_arraysize==0) {
        this->schedule = 0;
    } else {
        this->schedule = new int[schedule_arraysize];
        doParsimArrayUnpacking(b,this->schedule,schedule_arraysize);
    }
    doParsimUnpacking(b,this->remainingEnergy);
    doParsimUnpacking(b,this->sourceCH);
    doParsimUnpacking(b,this->isCH);
    doParsimUnpacking(b,this->nextCH);
    doParsimUnpacking(b,this->trace);
}

int konstant2019Packet::getKonstant2019PacketKind() const
{
    return this->konstant2019PacketKind;
}

void konstant2019Packet::setKonstant2019PacketKind(int konstant2019PacketKind)
{
    this->konstant2019PacketKind = konstant2019PacketKind;
}

int konstant2019Packet::getSourceId() const
{
    return this->sourceId;
}

void konstant2019Packet::setSourceId(int sourceId)
{
    this->sourceId = sourceId;
}

void konstant2019Packet::setScheduleArraySize(unsigned int size)
{
    int *schedule2 = (size==0) ? nullptr : new int[size];
    unsigned int sz = schedule_arraysize < size ? schedule_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        schedule2[i] = this->schedule[i];
    for (unsigned int i=sz; i<size; i++)
        schedule2[i] = 0;
    schedule_arraysize = size;
    delete [] this->schedule;
    this->schedule = schedule2;
}

unsigned int konstant2019Packet::getScheduleArraySize() const
{
    return schedule_arraysize;
}

int konstant2019Packet::getSchedule(unsigned int k) const
{
    if (k>=schedule_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", schedule_arraysize, k);
    return this->schedule[k];
}

void konstant2019Packet::setSchedule(unsigned int k, int schedule)
{
    if (k>=schedule_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", schedule_arraysize, k);
    this->schedule[k] = schedule;
}

double konstant2019Packet::getRemainingEnergy() const
{
    return this->remainingEnergy;
}

void konstant2019Packet::setRemainingEnergy(double remainingEnergy)
{
    this->remainingEnergy = remainingEnergy;
}

int konstant2019Packet::getSourceCH() const
{
    return this->sourceCH;
}

void konstant2019Packet::setSourceCH(int sourceCH)
{
    this->sourceCH = sourceCH;
}

IntVector& konstant2019Packet::getIsCH()
{
    return this->isCH;
}

void konstant2019Packet::setIsCH(const IntVector& isCH)
{
    this->isCH = isCH;
}

IntVector& konstant2019Packet::getNextCH()
{
    return this->nextCH;
}

void konstant2019Packet::setNextCH(const IntVector& nextCH)
{
    this->nextCH = nextCH;
}

IntVector& konstant2019Packet::getTrace()
{
    return this->trace;
}

void konstant2019Packet::setTrace(const IntVector& trace)
{
    this->trace = trace;
}

class konstant2019PacketDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    konstant2019PacketDescriptor();
    virtual ~konstant2019PacketDescriptor();

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

Register_ClassDescriptor(konstant2019PacketDescriptor)

konstant2019PacketDescriptor::konstant2019PacketDescriptor() : omnetpp::cClassDescriptor("konstant2019Packet", "RoutingPacket")
{
    propertynames = nullptr;
}

konstant2019PacketDescriptor::~konstant2019PacketDescriptor()
{
    delete[] propertynames;
}

bool konstant2019PacketDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<konstant2019Packet *>(obj)!=nullptr;
}

const char **konstant2019PacketDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *konstant2019PacketDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int konstant2019PacketDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 8+basedesc->getFieldCount() : 8;
}

unsigned int konstant2019PacketDescriptor::getFieldTypeFlags(int field) const
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
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
    };
    return (field>=0 && field<8) ? fieldTypeFlags[field] : 0;
}

const char *konstant2019PacketDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "konstant2019PacketKind",
        "sourceId",
        "schedule",
        "remainingEnergy",
        "sourceCH",
        "isCH",
        "nextCH",
        "trace",
    };
    return (field>=0 && field<8) ? fieldNames[field] : nullptr;
}

int konstant2019PacketDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='k' && strcmp(fieldName, "konstant2019PacketKind")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "sourceId")==0) return base+1;
    if (fieldName[0]=='s' && strcmp(fieldName, "schedule")==0) return base+2;
    if (fieldName[0]=='r' && strcmp(fieldName, "remainingEnergy")==0) return base+3;
    if (fieldName[0]=='s' && strcmp(fieldName, "sourceCH")==0) return base+4;
    if (fieldName[0]=='i' && strcmp(fieldName, "isCH")==0) return base+5;
    if (fieldName[0]=='n' && strcmp(fieldName, "nextCH")==0) return base+6;
    if (fieldName[0]=='t' && strcmp(fieldName, "trace")==0) return base+7;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *konstant2019PacketDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "int",
        "double",
        "int",
        "IntVector",
        "IntVector",
        "IntVector",
    };
    return (field>=0 && field<8) ? fieldTypeStrings[field] : nullptr;
}

const char **konstant2019PacketDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 0: {
            static const char *names[] = { "enum",  nullptr };
            return names;
        }
        default: return nullptr;
    }
}

const char *konstant2019PacketDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "konstant2019Packet_Type";
            return nullptr;
        default: return nullptr;
    }
}

int konstant2019PacketDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    konstant2019Packet *pp = (konstant2019Packet *)object; (void)pp;
    switch (field) {
        case 2: return pp->getScheduleArraySize();
        default: return 0;
    }
}

const char *konstant2019PacketDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    konstant2019Packet *pp = (konstant2019Packet *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string konstant2019PacketDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    konstant2019Packet *pp = (konstant2019Packet *)object; (void)pp;
    switch (field) {
        case 0: return enum2string(pp->getKonstant2019PacketKind(), "konstant2019Packet_Type");
        case 1: return long2string(pp->getSourceId());
        case 2: return long2string(pp->getSchedule(i));
        case 3: return double2string(pp->getRemainingEnergy());
        case 4: return long2string(pp->getSourceCH());
        case 5: {std::stringstream out; out << pp->getIsCH(); return out.str();}
        case 6: {std::stringstream out; out << pp->getNextCH(); return out.str();}
        case 7: {std::stringstream out; out << pp->getTrace(); return out.str();}
        default: return "";
    }
}

bool konstant2019PacketDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    konstant2019Packet *pp = (konstant2019Packet *)object; (void)pp;
    switch (field) {
        case 0: pp->setKonstant2019PacketKind((konstant2019Packet_Type)string2enum(value, "konstant2019Packet_Type")); return true;
        case 1: pp->setSourceId(string2long(value)); return true;
        case 2: pp->setSchedule(i,string2long(value)); return true;
        case 3: pp->setRemainingEnergy(string2double(value)); return true;
        case 4: pp->setSourceCH(string2long(value)); return true;
        default: return false;
    }
}

const char *konstant2019PacketDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 5: return omnetpp::opp_typename(typeid(IntVector));
        case 6: return omnetpp::opp_typename(typeid(IntVector));
        case 7: return omnetpp::opp_typename(typeid(IntVector));
        default: return nullptr;
    };
}

void *konstant2019PacketDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    konstant2019Packet *pp = (konstant2019Packet *)object; (void)pp;
    switch (field) {
        case 5: return (void *)(&pp->getIsCH()); break;
        case 6: return (void *)(&pp->getNextCH()); break;
        case 7: return (void *)(&pp->getTrace()); break;
        default: return nullptr;
    }
}


