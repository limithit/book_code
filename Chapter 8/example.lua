package.path = "../src/?.lua;src/?.lua;" .. package.path
pcall(require, "luarocks.require")             --��Ҫ���ǰ�װLuasocket��
local redis = require 'redis'
local params = {
    host = '127.0.0.1',
    port = 6379,
}
local client = redis.connect(params)                     -- ����redis
client:select(0) -- for testing purposes                   -- ѡ��db0��
client:drop_insert('192.168.1.1')           -- ��ͬiptables -I INPUT -s 192.168.1.1 -j DROP
client:drop_delete('192.168.1.1')           --ͬ��
client:ttl_drop_insert('192.168.1.2', '60')      --��������60����Զ�ɾ����ӵĹ���
local value = client:get('192.168.1.2')
print(value)
