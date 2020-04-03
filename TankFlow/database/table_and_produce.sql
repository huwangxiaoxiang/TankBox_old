use tank;
drop table if exists damageo;
create table damage(
	id varchar(18) primary key,
    attacker varchar(30) default "-1",
    victim varchar(30) default "-1",
    damages int default -1,
    damagetype int default -1,
    battletype int default -1,
    battleid int default -1,
    times timestamp default current_timestamp()
);

drop table if exists damage_ave_table;
create table damage_ave_table(
	id int primary key auto_increment ,
    tank_name varchar(30) not null,
    damage int default 0
);


drop table if exists assembly_update_table;
 create table assembly_update_table(
	id int primary key auto_increment ,
    versions varchar(20) not null,
    file_name varchar(200) not null,
    baidu_link varchar(200) ,
    passwords varchar(18) not null,
    description text,
    times timestamp default current_timestamp()
);

insert into assembly_update_table(file_name,versions,baidu_link,passwords,description) values("坦克大战盒子V1.4.0","v1.4.0","https:baidu.com","ok_myname","这是介绍");

-- 计算更新damage_ave_table表
drop procedure if exists update_damage_ave_table;
delimiter //
create procedure update_damage_ave_table()
begin
	declare name varchar(30);
    declare damages int default 0;
    declare cur cursor for select attacker,AVG(damage.damages) from damage group by attacker;
	declare continue handler for NOT FOUND set @have= true;
    set @have=false;
    
    set SQL_SAFE_UPDATES=0;
    truncate table damage_ave_table;
    
    set SQL_SAFE_UPDATES=1;
    
    open cur;
    fetch cur into name,damages;
    while(not @have) do
		-- call damage_ave(name,damages);
		insert into damage_ave_table(tank_name,damage) values (name,damages);
    fetch cur into name,damages;
    end while;
    close cur;
end
//
delimiter ;

drop procedure if exists add_damage;
delimiter //
create procedure add_damage(in attackers varchar(30),in victims varchar(30),in damagess int,
	in damagetypes int,in battletypes int ,in battleids int,out result int)
begin
	call create_id(@ids);
    insert into damage(id,attacker,victim,damages,damagetype,battletype,battleid) values(
		@ids,attackers,victims,damagess,damagetypes,battletypes,battleids);
	set result = 0;
end
//
delimiter ;

drop procedure if exists damage_ave;-- 某个坦克平均伤害函数 --
delimiter //
create procedure damage_ave(in tanks varchar(30),out result decimal)
begin
	select AVG(m.n) into result from (select damages as n from damage where attacker=tanks) as m;
end
//
delimiter ;

-- 坦克更名
drop procedure if exists rename_tank;
delimiter //
create procedure rename_tank(in old_name varchar(30),in new_name varchar(30),out result int)
begin
	set SQL_SAFE_UPDATES=0;
	update damage set attacker = new_name where attacker = old_name;
    update damage set victim = new_name where victim = old_name;
    set SQL_SAFE_UPDATES=1;
    set result=0;
end
//
delimiter ;




drop procedure if exists create_id;-- 创建ID号
delimiter //
create procedure create_id(out card varchar(20))
begin
	declare time varchar(18);
    declare count varchar(4);
    set time=DATE_FORMAT(NOW(),'%Y%m%d%k%i%s');
	set count=(CEIL(RAND() * 8000) + 1000);
    set card=CONCAT(time,count);
end
//
delimiter ;

use tank;
-- 定时任务 
SET GLOBAL event_scheduler = 1;
-- 每天更新平均伤害
drop event update_damage_ave_event;
create event update_damage_ave_event on schedule every 1 day starts "2019-12-06 00:00:00" do
	call tank.update_damage_ave_table();
ALTER EVENT update_damage_ave_event ON COMPLETION PRESERVE ENABLE;


