/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <openr/common/Types.h>
#include <openr/common/Util.h>
#include <openr/config/Config.h>
#include <optional>

namespace {

// utility function to construct thrift::AreaConfig
openr::thrift::AreaConfig
createAreaConfig(
    const std::string& areaId,
    const std::vector<std::string>& neighborRegexes,
    const std::vector<std::string>& interfaceRegexes,
    const std::optional<std::string>& policy = std::nullopt) {
  openr::thrift::AreaConfig areaConfig;
  areaConfig.area_id_ref() = areaId;
  areaConfig.neighbor_regexes_ref() = neighborRegexes;
  areaConfig.include_interface_regexes_ref() = interfaceRegexes;
  if (policy) {
    areaConfig.set_ingress_policy(*policy);
  }
  return areaConfig;
}

openr::thrift::OpenrConfig
getBasicOpenrConfig(
    const std::string nodeName = "",
    const std::string domainName = "domain",
    const std::vector<openr::thrift::AreaConfig>& areaCfg = {},
    bool enableV4 = true,
    bool enableSegmentRouting = false,
    bool dryrun = true,
    bool enableV4OverV6Nexthop = false) {
  openr::thrift::LinkMonitorConfig linkMonitorConfig;
  *linkMonitorConfig.include_interface_regexes_ref() =
      std::vector<std::string>{".*"};
  *linkMonitorConfig.redistribute_interface_regexes_ref() =
      std::vector<std::string>{"lo1"};

  openr::thrift::KvstoreConfig kvstoreConfig;

  openr::thrift::SparkConfig sparkConfig;
  *sparkConfig.hello_time_s_ref() = 2;
  *sparkConfig.keepalive_time_s_ref() = 1;
  *sparkConfig.fastinit_hello_time_ms_ref() = 50;
  *sparkConfig.hold_time_s_ref() = 2;
  *sparkConfig.graceful_restart_time_s_ref() = 6;

  openr::thrift::OpenrConfig config;

  *config.node_name_ref() = nodeName;
  *config.domain_ref() = domainName;
  config.enable_v4_ref() = enableV4;
  config.v4_over_v6_nexthop_ref() = enableV4OverV6Nexthop;
  config.enable_segment_routing_ref() = enableSegmentRouting;
  config.dryrun_ref() = dryrun;

  *config.kvstore_config_ref() = kvstoreConfig;
  *config.link_monitor_config_ref() = linkMonitorConfig;
  *config.spark_config_ref() = sparkConfig;

  *config.enable_rib_policy_ref() = true;

  if (areaCfg.empty()) {
    config.areas_ref() = {createAreaConfig(kTestingAreaName, {".*"}, {".*"})};
  } else {
    config.areas_ref() = areaCfg;
  }

  return config;
}

} // namespace
