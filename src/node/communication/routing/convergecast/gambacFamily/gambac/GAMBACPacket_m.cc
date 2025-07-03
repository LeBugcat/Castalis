//
// Generated file, do not edit! Created by nedtool 5.6 from node/communication/routing/cluster/gambac/GAMBACPacket.msg.
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
#include "GAMBACPacket_m.h"

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
    omnetpp::cEnum *e = omnetpp::cEnum::find("GAMBACPacket_Type");
    if (!e) omnetpp::enums.getInstance()->add(e = new omnetpp::cEnum("GAMBACPacket_Type"));
    e->insert(GAMBAC_ROUTING_CONTROL_PACKET, "GAMBAC_ROUTING_CONTROL_PACKET");
    e->insert(GAMBAC_ROUTING_DATA_PACKET, "GAMBAC_ROUTING_DATA_PACKET");
    e->insert(GAMBAC_ROUTING_JOIN_PACKET, "GAMBAC_ROUTING_JOIN_PACKET");
    e->insert(GAMBAC_ROUTING_ADV_PACKET, "GAMBAC_ROUTING_ADV_PACKET");
    e->insert(DATA_PACKET_FROM_MAC, "DATA_PACKET_FROM_MAC");
)

Register_Class(GAMBACPacket)

GAMBACPacket::GAMBACPacket(const char *name, short kind) : ::RoutingPacket(name,kind)
{
    this->GAMBACPacketKind = 0;
    this->sourceCH = 0;
    schedule_arraysize = 0;
    this->schedule = 0;
    this->remainingEnergy = 0;
}

GAMBACPacket::GAMBACPacket(const GAMBACPacket& other) : ::RoutingPacket(other)
{
    schedule_arraysize = 0;
    this->schedule = 0;
    copy(other);
}

GAMBACPacket::~GAMBACPacket()
{
    delete [] this->schedule;
}

GAMBACPacket& GAMBACPacket::operator=(const GAMBACPacket& other)
{
    if (this==&other) return *this;
    ::RoutingPacket::operator=(other);
    copy(other);
    return *this;
}

void GAMBACPacket::copy(const GAMBACPacket& other)
{
    this->GAMBACPacketKind = other.GAMBACPacketKind;
    this->sourceCH = other.sourceCH;
    delete [] this->schedule;
    this->schedule = (other.schedule_arraysize==0) ? nullptr : new int[other.schedule_arraysize];
    schedule_arraysize = other.schedule_arraysize;
    for (unsigned int i=0; i<schedule_arraysize; i++)
        this->schedule[i] = other.schedule[i];
    this->remainingEnergy = other.remainingEnergy;
    this->isCH = other.isCH;
    this->nextCH = other.nextCH;
    this->trace = other.trace;
}

void GAMBACPacket::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::RoutingPacket::parsimPack(b);
    doParsimPacking(b,this->GAMBACPacketKind);
    doParsimPacking(b,this->sourceCH);
    b->pack(schedule_arraysize);
    doParsimArrayPacking(b,this->schedule,schedule_arraysize);
    doParsimPacking(b,this->remainingEnergy);
    doParsimPacking(b,this->isCH);
    doParsimPacking(b,this->nextCH);
    doParsimPacking(b,this->trace);
}

void GAMBACPacket::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::RoutingPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->GAMBACPacketKind);
    doParsimUnpacking(b,this->sourceCH);
    delete [] this->schedule;
    b->unpack(schedule_arraysize);
    if (schedule_arraysize==0) {
        this->schedule = 0;
    } else {
        this->schedule = new int[schedule_arraysize];
        doParsimArrayUnpacking(b,this->schedule,schedule_arraysize);
    }
    doParsimUnpacking(b,this->remainingEnergy);
    doParsimUnpacking(b,this->isCH);
    doParsimUnpacking(b,this->nextCH);
    doParsimUnpacking(b,this->trace);
}

int GAMBACPacket::getGAMBACPacketKind() const
{
    return this->GAMBACPacketKind;
}

void GAMBACPacket::setGAMBACPacketKind(int GAMBACPacketKind)
{
    this->GAMBACPacketKind = GAMBACPacketKind;
}

int GAMBACPacket::getSourceCH() const
{
    return this->sourceCH;
}

void GAMBACPacket::setSourceCH(int sourceCH)
{
    this->sourceCH = sourceCH;
}

void GAMBACPacket::setScheduleArraySize(unsigned int size)
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

unsigned int GAMBACPacket::getScheduleArraySize() const
{
    return schedule_arraysize;
}

int GAMBACPacket::getSchedule(unsigned int k) const
{
    if (k>=schedule_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", schedule_arraysize, k);
    return this->schedule[k];
}

void GAMBACPacket::setSchedule(unsigned int k, int schedule)
{
    if (k>=schedule_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", schedule_arraysize, k);
    this->schedule[k] = schedule;
}

double GAMBACPacket::getRemainingEnergy() const
{
    return this->remainingEnergy;
}

void GAMBACPacket::setRemainingEnergy(double remainingEnergy)
{
    this->remainingEnergy = remainingEnergy;
}

IntVector& GAMBACPacket::getIsCH()
{
    return this->isCH;
}

void GAMBACPacket::setIsCH(const IntVector& isCH)
{
    this->isCH = isCH;
}

IntVector& GAMBACPacket::getNextCH()
{
    return this->nextCH;
}

void GAMBACPacket::setNextCH(const IntVector& nextCH)
{
    this->nextCH = nextCH;
}

IntVector& GAMBACPacket::getTrace()
{
    return this->trace;
}

void GAMBACPacket::setTrace(const IntVector& trace)
{
    this->trace = trace;
}

class GAMBACPacketDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    GAMBACPacketDescriptor();
    virtual ~GAMBACPacketDescriptor();

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

Register_ClassDescriptor(GAMBACPacketDescriptor)

GAMBACPacketDescriptor::GAMBACPacketDescriptor() : omnetpp::cClassDescriptor("GAMBACPacket", "RoutingPacket")
{
    propertynames = nullptr;
}

GAMBACPacketDescriptor::~GAMBACPacketDescriptor()
{
    delete[] propertynames;
}

bool GAMBACPacketDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<GAMBACPacket *>(obj)!=nullptr;
}

const char **GAMBACPacketDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *GAMBACPacketDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int GAMBACPacketDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 7+basedesc->getFieldCount() : 7;
}

unsigned int GAMBACPacketDescriptor::getFieldTypeFlags(int field) const
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
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
    };
    return (field>=0 && field<7) ? fieldTypeFlags[field] : 0;
}

const char *GAMBACPacketDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "GAMBACPacketKind",
        "sourceCH",
        "schedule",
        "remainingEnergy",
        "isCH",
        "nextCH",
        "trace",
    };
    return (field>=0 && field<7) ? fieldNames[field] : nullptr;
}

int GAMBACPacketDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='G' && strcmp(fieldName, "GAMBACPacketKind")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "sourceCH")==0) return base+1;
    if (fieldName[0]=='s' && strcmp(fieldName, "schedule")==0) return base+2;
    if (fieldName[0]=='r' && strcmp(fieldName, "remainingEnergy")==0) return base+3;
    if (fieldName[0]=='i' && strcmp(fieldName, "isCH")==0) return base+4;
    if (fieldName[0]=='n' && strcmp(fieldName, "nextCH")==0) return base+5;
    if (fieldName[0]=='t' && strcmp(fieldName, "trace")==0) return base+6;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *GAMBACPacketDescriptor::getFieldTypeString(int field) const
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
        "IntVector",
        "IntVector",
        "IntVector",
    };
    return (field>=0 && field<7) ? fieldTypeStrings[field] : nullptr;
}

const char **GAMBACPacketDescriptor::getFieldPropertyNames(int field) const
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

const char *GAMBACPacketDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "GAMBACPacket_Type";
            return nullptr;
        default: return nullptr;
    }
}

int GAMBACPacketDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    GAMBACPacket *pp = (GAMBACPacket *)object; (void)pp;
    switch (field) {
        case 2: return pp->getScheduleArraySize();
        default: return 0;
    }
}

const char *GAMBACPacketDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    GAMBACPacket *pp = (GAMBACPacket *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string GAMBACPacketDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    GAMBACPacket *pp = (GAMBACPacket *)object; (void)pp;
    switch (field) {
        case 0: return enum2string(pp->getGAMBACPacketKind(), "GAMBACPacket_Type");
        case 1: return long2string(pp->getSourceCH());
        case 2: return long2string(pp->getSchedule(i));
        case 3: return double2string(pp->getRemainingEnergy());
        case 4: {std::stringstream out; out << pp->getIsCH(); return out.str();}
        case 5: {std::stringstream out; out << pp->getNextCH(); return out.str();}
        case 6: {std::stringstream out; out << pp->getTrace(); return out.str();}
        default: return "";
    }
}

bool GAMBACPacketDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    GAMBACPacket *pp = (GAMBACPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setGAMBACPacketKind((GAMBACPacket_Type)string2enum(value, "GAMBACPacket_Type")); return true;
        case 1: pp->setSourceCH(string2long(value)); return true;
        case 2: pp->setSchedule(i,string2long(value)); return true;
        case 3: pp->setRemainingEnergy(string2double(value)); return true;
        default: return false;
    }
}

const char *GAMBACPacketDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 4: return omnetpp::opp_typename(typeid(IntVector));
        case 5: return omnetpp::opp_typename(typeid(IntVector));
        case 6: return omnetpp::opp_typename(typeid(IntVector));
        default: return nullptr;
    };
}

void *GAMBACPacketDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    GAMBACPacket *pp = (GAMBACPacket *)object; (void)pp;
    switch (field) {
        case 4: return (void *)(&pp->getIsCH()); break;
        case 5: return (void *)(&pp->getNextCH()); break;
        case 6: return (void *)(&pp->getTrace()); break;
        default: return nullptr;
    }
}


