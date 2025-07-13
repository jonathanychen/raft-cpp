#ifndef messages_h
#define messages_h

#include "nlohmann/detail/macro_scope.hpp"
#include <nlohmann/json.hpp>

struct BaseClientMessage {
  std::string src;
  std::string dst;
  std::string leader;
  std::string MID;
  std::string type;
};

struct GetRequest : public BaseClientMessage {
  std::string key;
  std::string type = "get";
};

struct GetOkResponse : public BaseClientMessage {
  std::string type = "ok";
  std::string key;
  std::string value;
};

struct GetFailResponse : public BaseClientMessage {
  std::string type = "fail";
};

struct PutRequest : public BaseClientMessage {
  std::string key;
  std::string value;
  std::string type = "put";
};

struct PutOkResponse : public BaseClientMessage {
  std::string type = "ok";
};

struct PutFailResponse : public BaseClientMessage {
  std::string type = "fail";
};

struct RedirectResponse : public BaseClientMessage {
  std::string type = "redirect";
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GetRequest, src, dst, leader, MID, type,
                                   key);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GetOkResponse, src, dst, leader, MID, type,
                                   value);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GetFailResponse, src, dst, leader, MID,
                                   type);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PutRequest, src, dst, leader, MID, type, key,
                                   value);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PutOkResponse, src, dst, leader, MID, type);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PutFailResponse, src, dst, leader, MID,
                                   type);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(RedirectResponse, src, dst, leader, MID,
                                   type);

#endif
