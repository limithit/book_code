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
  `ip` varchar(50) DEFAULT NULL COMMENT '������IP',
  `server` varchar(255) DEFAULT NULL COMMENT '��������',
  `attack_type` varchar(24) DEFAULT NULL COMMENT '��������',
  `score` int(255) DEFAULT NULL,
  `url` char(255) DEFAULT NULL,
  `at` datetime NOT NULL COMMENT '����ʱ��',
  `is_send` int(10) DEFAULT '10' COMMENT '�Ƿ��ѷ��� 10δ���� 20�ѷ���',
  PRIMARY KEY (`id`),
  KEY `host` (`ip`,`at`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=404009 DEFAULT CHARSET=utf8;