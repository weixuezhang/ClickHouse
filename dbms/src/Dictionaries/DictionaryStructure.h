#pragma once

#include <DataTypes/IDataType.h>
#include <IO/ReadBufferFromString.h>
#include <Interpreters/IExternalLoadable.h>
#include <Poco/Util/AbstractConfiguration.h>

#include <vector>
#include <string>
#include <map>
#include <optional>


namespace DB
{

enum class AttributeUnderlyingType
{
    UInt8,
    UInt16,
    UInt32,
    UInt64,
    UInt128,
    Int8,
    Int16,
    Int32,
    Int64,
    Float32,
    Float64,
    Decimal32,
    Decimal64,
    Decimal128,
    String
};


/** For implicit conversions in dictGet functions.
  */
bool isAttributeTypeConvertibleTo(AttributeUnderlyingType from, AttributeUnderlyingType to);

AttributeUnderlyingType getAttributeUnderlyingType(const std::string & type);

std::string toString(const AttributeUnderlyingType type);


/// Min and max lifetimes for a dictionary or it's entry
using DictionaryLifetime = ExternalLoadableLifetime;


/** Holds the description of a single dictionary attribute:
*    - name, used for lookup into dictionary and source;
*    - type, used in conjunction with DataTypeFactory and getAttributeUnderlyingTypeByname;
*    - null_value, used as a default value for non-existent entries in the dictionary,
*        decimal representation for numeric attributes;
*    - hierarchical, whether this attribute defines a hierarchy;
*    - injective, whether the mapping to parent is injective (can be used for optimization of GROUP BY?)
*    - is_object_id, used in mongo dictionary, converts string key to objectid
*/
struct DictionaryAttribute final
{
    const std::string name;
    const AttributeUnderlyingType underlying_type;
    const DataTypePtr type;
    const std::string expression;
    const Field null_value;
    const bool hierarchical;
    const bool injective;
    const bool is_object_id;
};


struct DictionarySpecialAttribute final
{
    const std::string name;
    const std::string expression;

    DictionarySpecialAttribute(const Poco::Util::AbstractConfiguration & config, const std::string & config_prefix);
};

struct DictionaryTypedSpecialAttribute final
{
    const std::string name;
    const std::string expression;
    const DataTypePtr type;
};


/// Name of identifier plus list of attributes
struct DictionaryStructure final
{
    std::optional<DictionarySpecialAttribute> id;
    std::optional<std::vector<DictionaryAttribute>> key;
    std::vector<DictionaryAttribute> attributes;
    std::optional<DictionaryTypedSpecialAttribute> range_min;
    std::optional<DictionaryTypedSpecialAttribute> range_max;
    bool has_expressions = false;

    DictionaryStructure(const Poco::Util::AbstractConfiguration & config, const std::string & config_prefix);

    void validateKeyTypes(const DataTypes & key_types) const;
    std::string getKeyDescription() const;
    bool isKeySizeFixed() const;
    size_t getKeySize() const;

private:
    std::vector<DictionaryAttribute> getAttributes(
        const Poco::Util::AbstractConfiguration & config, const std::string & config_prefix,
        const bool hierarchy_allowed = true, const bool allow_null_values = true);
};

}
