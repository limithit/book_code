package.path = "../src/?.lua;src/?.lua;" .. package.path
pcall(require, "luarocks.require")             --不要忘记安装Luasocket库
local redis = require 'redis'
local params = {
    host = '127.0.0.1',
    port = 6379,
}
local client = redis.connect(params)                     -- 连接redis
client:select(0) -- for testing purposes                   -- 选择db0库
client:drop_insert('192.168.1.1')           -- 等同iptables -I INPUT -s 192.168.1.1 -j DROP
client:drop_delete('192.168.1.1')           --同上
client:ttl_drop_insert('192.168.1.2', '60')      --加入规则后60秒后自动删除添加的规则
local value = client:get('192.168.1.2')
print(value)
