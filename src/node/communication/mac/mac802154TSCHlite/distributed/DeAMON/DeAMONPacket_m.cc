//
// Generated file, do not edit! Created by nedtool 5.6 from node/communication/mac/mac802154TSCHlite/distributed/DeAMON/DeAMONPacket.msg.
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
#include "DeAMONPacket_m.h"

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
    omnetpp::cEnum *e = omnetpp::cEnum::find("DeAMONPacket_type");
    if (!e) omnetpp::enums.getInstance()->add(e = new omnetpp::cEnum("DeAMONPacket_type"));
    e->insert(BUILD, "BUILD");
    e->insert(RFS, "RFS");
    e->insert(RFS_OVERHEARD, "RFS_OVERHEARD");
    e->insert(ASG, "ASG");
    e->insert(ASG_OVERHEARD, "ASG_OVERHEARD");
    e->insert(COLLISION, "COLLISION");
)

Register_Class(DeAMONPacket)

DeAMONPacket::DeAMONPacket(const char *name, short kind) : ::MAC802154LitePacket(name,kind)
{
    this->DeAMONPacketType = 0;
    childrenListByPriority_arraysize = 0;
    this->childrenListByPriority = 0;
    this->signallingTimeslot = 0;
    this->signallingChannel = 0;
    RFSTimeslots_arraysize = 0;
    this->RFSTimeslots = 0;
    this->RFSChannel = 0;
    ASGTimeslots_arraysize = 0;
    this->ASGTimeslots = 0;
    ASGChannels_arraysize = 0;
    this->ASGChannels = 0;
}

DeAMONPacket::DeAMONPacket(const DeAMONPacket& other) : ::MAC802154LitePacket(other)
{
    childrenListByPriority_arraysize = 0;
    this->childrenListByPriority = 0;
    RFSTimeslots_arraysize = 0;
    this->RFSTimeslots = 0;
    ASGTimeslots_arraysize = 0;
    this->ASGTimeslots = 0;
    ASGChannels_arraysize = 0;
    this->ASGChannels = 0;
    copy(other);
}

DeAMONPacket::~DeAMONPacket()
{
    delete [] this->childrenListByPriority;
    delete [] this->RFSTimeslots;
    delete [] this->ASGTimeslots;
    delete [] this->ASGChannels;
}

DeAMONPacket& DeAMONPacket::operator=(const DeAMONPacket& other)
{
    if (this==&other) return *this;
    ::MAC802154LitePacket::operator=(other);
    copy(other);
    return *this;
}

void DeAMONPacket::copy(const DeAMONPacket& other)
{
    this->DeAMONPacketType = other.DeAMONPacketType;
    delete [] this->childrenListByPriority;
    this->childrenListByPriority = (other.childrenListByPriority_arraysize==0) ? nullptr : new int[other.childrenListByPriority_arraysize];
    childrenListByPriority_arraysize = other.childrenListByPriority_arraysize;
    for (unsigned int i=0; i<childrenListByPriority_arraysize; i++)
        this->childrenListByPriority[i] = other.childrenListByPriority[i];
    this->signallingTimeslot = other.signallingTimeslot;
    this->signallingChannel = other.signallingChannel;
    delete [] this->RFSTimeslots;
    this->RFSTimeslots = (other.RFSTimeslots_arraysize==0) ? nullptr : new int[other.RFSTimeslots_arraysize];
    RFSTimeslots_arraysize = other.RFSTimeslots_arraysize;
    for (unsigned int i=0; i<RFSTimeslots_arraysize; i++)
        this->RFSTimeslots[i] = other.RFSTimeslots[i];
    this->RFSChannel = other.RFSChannel;
    delete [] this->ASGTimeslots;
    this->ASGTimeslots = (other.ASGTimeslots_arraysize==0) ? nullptr : new int[other.ASGTimeslots_arraysize];
    ASGTimeslots_arraysize = other.ASGTimeslots_arraysize;
    for (unsigned int i=0; i<ASGTimeslots_arraysize; i++)
        this->ASGTimeslots[i] = other.ASGTimeslots[i];
    delete [] this->ASGChannels;
    this->ASGChannels = (other.ASGChannels_arraysize==0) ? nullptr : new int[other.ASGChannels_arraysize];
    ASGChannels_arraysize = other.ASGChannels_arraysize;
    for (unsigned int i=0; i<ASGChannels_arraysize; i++)
        this->ASGChannels[i] = other.ASGChannels[i];
}

void DeAMONPacket::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::MAC802154LitePacket::parsimPack(b);
    doParsimPacking(b,this->DeAMONPacketType);
    b->pack(childrenListByPriority_arraysize);
    doParsimArrayPacking(b,this->childrenListByPriority,childrenListByPriority_arraysize);
    doParsimPacking(b,this->signallingTimeslot);
    doParsimPacking(b,this->signallingChannel);
    b->pack(RFSTimeslots_arraysize);
    doParsimArrayPacking(b,this->RFSTimeslots,RFSTimeslots_arraysize);
    doParsimPacking(b,this->RFSChannel);
    b->pack(ASGTimeslots_arraysize);
    doParsimArrayPacking(b,this->ASGTimeslots,ASGTimeslots_arraysize);
    b->pack(ASGChannels_arraysize);
    doParsimArrayPacking(b,this->ASGChannels,ASGChannels_arraysize);
}

void DeAMONPacket::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::MAC802154LitePacket::parsimUnpack(b);
    doParsimUnpacking(b,this->DeAMONPacketType);
    delete [] this->childrenListByPriority;
    b->unpack(childrenListByPriority_arraysize);
    if (childrenListByPriority_arraysize==0) {
        this->childrenListByPriority = 0;
    } else {
        this->childrenListByPriority = new int[childrenListByPriority_arraysize];
        doParsimArrayUnpacking(b,this->childrenListByPriority,childrenListByPriority_arraysize);
    }
    doParsimUnpacking(b,this->signallingTimeslot);
    doParsimUnpacking(b,this->signallingChannel);
    delete [] this->RFSTimeslots;
    b->unpack(RFSTimeslots_arraysize);
    if (RFSTimeslots_arraysize==0) {
        this->RFSTimeslots = 0;
    } else {
        this->RFSTimeslots = new int[RFSTimeslots_arraysize];
        doParsimArrayUnpacking(b,this->RFSTimeslots,RFSTimeslots_arraysize);
    }
    doParsimUnpacking(b,this->RFSChannel);
    delete [] this->ASGTimeslots;
    b->unpack(ASGTimeslots_arraysize);
    if (ASGTimeslots_arraysize==0) {
        this->ASGTimeslots = 0;
    } else {
        this->ASGTimeslots = new int[ASGTimeslots_arraysize];
        doParsimArrayUnpacking(b,this->ASGTimeslots,ASGTimeslots_arraysize);
    }
    delete [] this->ASGChannels;
    b->unpack(ASGChannels_arraysize);
    if (ASGChannels_arraysize==0) {
        this->ASGChannels = 0;
    } else {
        this->ASGChannels = new int[ASGChannels_arraysize];
        doParsimArrayUnpacking(b,this->ASGChannels,ASGChannels_arraysize);
    }
}

int DeAMONPacket::getDeAMONPacketType() const
{
    return this->DeAMONPacketType;
}

void DeAMONPacket::setDeAMONPacketType(int DeAMONPacketType)
{
    this->DeAMONPacketType = DeAMONPacketType;
}

void DeAMONPacket::setChildrenListByPriorityArraySize(unsigned int size)
{
    int *childrenListByPriority2 = (size==0) ? nullptr : new int[size];
    unsigned int sz = childrenListByPriority_arraysize < size ? childrenListByPriority_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        childrenListByPriority2[i] = this->childrenListByPriority[i];
    for (unsigned int i=sz; i<size; i++)
        childrenListByPriority2[i] = 0;
    childrenListByPriority_arraysize = size;
    delete [] this->childrenListByPriority;
    this->childrenListByPriority = childrenListByPriority2;
}

unsigned int DeAMONPacket::getChildrenListByPriorityArraySize() const
{
    return childrenListByPriority_arraysize;
}

int DeAMONPacket::getChildrenListByPriority(unsigned int k) const
{
    if (k>=childrenListByPriority_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", childrenListByPriority_arraysize, k);
    return this->childrenListByPriority[k];
}

void DeAMONPacket::setChildrenListByPriority(unsigned int k, int childrenListByPriority)
{
    if (k>=childrenListByPriority_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", childrenListByPriority_arraysize, k);
    this->childrenListByPriority[k] = childrenListByPriority;
}

int DeAMONPacket::getSignallingTimeslot() const
{
    return this->signallingTimeslot;
}

void DeAMONPacket::setSignallingTimeslot(int signallingTimeslot)
{
    this->signallingTimeslot = signallingTimeslot;
}

int DeAMONPacket::getSignallingChannel() const
{
    return this->signallingChannel;
}

void DeAMONPacket::setSignallingChannel(int signallingChannel)
{
    this->signallingChannel = signallingChannel;
}

void DeAMONPacket::setRFSTimeslotsArraySize(unsigned int size)
{
    int *RFSTimeslots2 = (size==0) ? nullptr : new int[size];
    unsigned int sz = RFSTimeslots_arraysize < size ? RFSTimeslots_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        RFSTimeslots2[i] = this->RFSTimeslots[i];
    for (unsigned int i=sz; i<size; i++)
        RFSTimeslots2[i] = 0;
    RFSTimeslots_arraysize = size;
    delete [] this->RFSTimeslots;
    this->RFSTimeslots = RFSTimeslots2;
}

unsigned int DeAMONPacket::getRFSTimeslotsArraySize() const
{
    return RFSTimeslots_arraysize;
}

int DeAMONPacket::getRFSTimeslots(unsigned int k) const
{
    if (k>=RFSTimeslots_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", RFSTimeslots_arraysize, k);
    return this->RFSTimeslots[k];
}

void DeAMONPacket::setRFSTimeslots(unsigned int k, int RFSTimeslots)
{
    if (k>=RFSTimeslots_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", RFSTimeslots_arraysize, k);
    this->RFSTimeslots[k] = RFSTimeslots;
}

int DeAMONPacket::getRFSChannel() const
{
    return this->RFSChannel;
}

void DeAMONPacket::setRFSChannel(int RFSChannel)
{
    this->RFSChannel = RFSChannel;
}

void DeAMONPacket::setASGTimeslotsArraySize(unsigned int size)
{
    int *ASGTimeslots2 = (size==0) ? nullptr : new int[size];
    unsigned int sz = ASGTimeslots_arraysize < size ? ASGTimeslots_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        ASGTimeslots2[i] = this->ASGTimeslots[i];
    for (unsigned int i=sz; i<size; i++)
        ASGTimeslots2[i] = 0;
    ASGTimeslots_arraysize = size;
    delete [] this->ASGTimeslots;
    this->ASGTimeslots = ASGTimeslots2;
}

unsigned int DeAMONPacket::getASGTimeslotsArraySize() const
{
    return ASGTimeslots_arraysize;
}

int DeAMONPacket::getASGTimeslots(unsigned int k) const
{
    if (k>=ASGTimeslots_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", ASGTimeslots_arraysize, k);
    return this->ASGTimeslots[k];
}

void DeAMONPacket::setASGTimeslots(unsigned int k, int ASGTimeslots)
{
    if (k>=ASGTimeslots_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", ASGTimeslots_arraysize, k);
    this->ASGTimeslots[k] = ASGTimeslots;
}

void DeAMONPacket::setASGChannelsArraySize(unsigned int size)
{
    int *ASGChannels2 = (size==0) ? nullptr : new int[size];
    unsigned int sz = ASGChannels_arraysize < size ? ASGChannels_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        ASGChannels2[i] = this->ASGChannels[i];
    for (unsigned int i=sz; i<size; i++)
        ASGChannels2[i] = 0;
    ASGChannels_arraysize = size;
    delete [] this->ASGChannels;
    this->ASGChannels = ASGChannels2;
}

unsigned int DeAMONPacket::getASGChannelsArraySize() const
{
    return ASGChannels_arraysize;
}

int DeAMONPacket::getASGChannels(unsigned int k) const
{
    if (k>=ASGChannels_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", ASGChannels_arraysize, k);
    return this->ASGChannels[k];
}

void DeAMONPacket::setASGChannels(unsigned int k, int ASGChannels)
{
    if (k>=ASGChannels_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", ASGChannels_arraysize, k);
    this->ASGChannels[k] = ASGChannels;
}

class DeAMONPacketDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    DeAMONPacketDescriptor();
    virtual ~DeAMONPacketDescriptor();

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

Register_ClassDescriptor(DeAMONPacketDescriptor)

DeAMONPacketDescriptor::DeAMONPacketDescriptor() : omnetpp::cClassDescriptor("DeAMONPacket", "MAC802154LitePacket")
{
    propertynames = nullptr;
}

DeAMONPacketDescriptor::~DeAMONPacketDescriptor()
{
    delete[] propertynames;
}

bool DeAMONPacketDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<DeAMONPacket *>(obj)!=nullptr;
}

const char **DeAMONPacketDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *DeAMONPacketDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int DeAMONPacketDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 8+basedesc->getFieldCount() : 8;
}

unsigned int DeAMONPacketDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
    };
    return (field>=0 && field<8) ? fieldTypeFlags[field] : 0;
}

const char *DeAMONPacketDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "DeAMONPacketType",
        "childrenListByPriority",
        "signallingTimeslot",
        "signallingChannel",
        "RFSTimeslots",
        "RFSChannel",
        "ASGTimeslots",
        "ASGChannels",
    };
    return (field>=0 && field<8) ? fieldNames[field] : nullptr;
}

int DeAMONPacketDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='D' && strcmp(fieldName, "DeAMONPacketType")==0) return base+0;
    if (fieldName[0]=='c' && strcmp(fieldName, "childrenListByPriority")==0) return base+1;
    if (fieldName[0]=='s' && strcmp(fieldName, "signallingTimeslot")==0) return base+2;
    if (fieldName[0]=='s' && strcmp(fieldName, "signallingChannel")==0) return base+3;
    if (fieldName[0]=='R' && strcmp(fieldName, "RFSTimeslots")==0) return base+4;
    if (fieldName[0]=='R' && strcmp(fieldName, "RFSChannel")==0) return base+5;
    if (fieldName[0]=='A' && strcmp(fieldName, "ASGTimeslots")==0) return base+6;
    if (fieldName[0]=='A' && strcmp(fieldName, "ASGChannels")==0) return base+7;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *DeAMONPacketDescriptor::getFieldTypeString(int field) const
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
        "int",
        "int",
        "int",
        "int",
        "int",
    };
    return (field>=0 && field<8) ? fieldTypeStrings[field] : nullptr;
}

const char **DeAMONPacketDescriptor::getFieldPropertyNames(int field) const
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

const char *DeAMONPacketDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "DeAMONPacket_type";
            return nullptr;
        default: return nullptr;
    }
}

int DeAMONPacketDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    DeAMONPacket *pp = (DeAMONPacket *)object; (void)pp;
    switch (field) {
        case 1: return pp->getChildrenListByPriorityArraySize();
        case 4: return pp->getRFSTimeslotsArraySize();
        case 6: return pp->getASGTimeslotsArraySize();
        case 7: return pp->getASGChannelsArraySize();
        default: return 0;
    }
}

const char *DeAMONPacketDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    DeAMONPacket *pp = (DeAMONPacket *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string DeAMONPacketDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    DeAMONPacket *pp = (DeAMONPacket *)object; (void)pp;
    switch (field) {
        case 0: return enum2string(pp->getDeAMONPacketType(), "DeAMONPacket_type");
        case 1: return long2string(pp->getChildrenListByPriority(i));
        case 2: return long2string(pp->getSignallingTimeslot());
        case 3: return long2string(pp->getSignallingChannel());
        case 4: return long2string(pp->getRFSTimeslots(i));
        case 5: return long2string(pp->getRFSChannel());
        case 6: return long2string(pp->getASGTimeslots(i));
        case 7: return long2string(pp->getASGChannels(i));
        default: return "";
    }
}

bool DeAMONPacketDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    DeAMONPacket *pp = (DeAMONPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setDeAMONPacketType((DeAMONPacket_type)string2enum(value, "DeAMONPacket_type")); return true;
        case 1: pp->setChildrenListByPriority(i,string2long(value)); return true;
        case 2: pp->setSignallingTimeslot(string2long(value)); return true;
        case 3: pp->setSignallingChannel(string2long(value)); return true;
        case 4: pp->setRFSTimeslots(i,string2long(value)); return true;
        case 5: pp->setRFSChannel(string2long(value)); return true;
        case 6: pp->setASGTimeslots(i,string2long(value)); return true;
        case 7: pp->setASGChannels(i,string2long(value)); return true;
        default: return false;
    }
}

const char *DeAMONPacketDescriptor::getFieldStructName(int field) const
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

void *DeAMONPacketDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    DeAMONPacket *pp = (DeAMONPacket *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}


