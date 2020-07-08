/*
Navicat MySQL Data Transfer
Source Database       : audit_sec
Target Server Type    : MYSQL
Target Server Version : 50635
File Encoding         : 65001
Date: 2019-03-12 09:48:50
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for naxsi_attack_log
-- ----------------------------
DROP TABLE IF EXISTS `naxsi_attack_log`;
CREATE TABLE `naxsi_attack_log` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `ip` varchar(50) DEFAULT NULL COMMENT '攻击者IP',
  `server` varchar(255) DEFAULT NULL COMMENT '攻击域名',
  `attack_type` varchar(24) DEFAULT NULL COMMENT '攻击类型',
  `score` int(255) DEFAULT NULL,
  `url` char(255) DEFAULT NULL,
  `at` datetime NOT NULL COMMENT '创建时间',
  `is_send` int(10) DEFAULT '10' COMMENT '是否已发送 10未发送 20已发送',
  PRIMARY KEY (`id`),
  KEY `host` (`ip`,`at`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=404009 DEFAULT CHARSET=utf8;