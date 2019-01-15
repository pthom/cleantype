#include "doctest.h"
#include <cleantype/cleantype.hpp>
#include <cleantype/cleantype_configuration.hpp>
#include <nlohmann/json.hpp>
#include <utility>

#define LOG_VAR(var) std::cout << #var << "\n" << var << "\n";

TEST_CASE("CleanConfiguration_string_serialize")
{
    using namespace nlohmann;
    cleantype::CleanConfiguration config_current = cleantype::CleanConfiguration::GlobalConfig();
    json json_data_current = config_current;

    std::string str_example = cleantype::CleanConfigurationExample;
    json json_data_example = json::parse(str_example);

    REQUIRE_EQ(json_data_current, json_data_example);

    cleantype::CleanConfiguration config_example = json_data_example.get<cleantype::CleanConfiguration>();
    REQUIRE_EQ(
        config_current.suppress_extra_namespaces_,
        config_example.suppress_extra_namespaces_
    );
}
