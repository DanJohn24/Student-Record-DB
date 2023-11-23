DROP TABLE IF EXISTS `student_table`;

CREATE TABLE `student_table` (
  `student_id` mediumint(8) unsigned NOT NULL auto_increment,
  `student_name` varchar(255) default NULL,
  PRIMARY KEY (`student_id`)
) AUTO_INCREMENT=1;

INSERT INTO `student_table` (`student_name`)
VALUES
  ("Whilemina"),
  ("Riley"),
  ("Kathleen"),
  ("Brenda"),
  ("Kalia");

DROP TABLE IF EXISTS `module_list_table`;

CREATE TABLE `module_list_table` (
  `module_id` mediumint(8) unsigned NOT NULL auto_increment,
  `module_name` varchar(255) default NULL,
  `assinged_user` varchar(255) default NULL,
  PRIMARY KEY (`module_id`)
) AUTO_INCREMENT=1;

INSERT INTO `module_list_table` (`module_name`)
VALUES
  ("maths"),
  ("science"),
  ("english");

  
DROP TABLE IF EXISTS `maths`;

CREATE TABLE `maths` (
  `id` mediumint(8) unsigned NOT NULL auto_increment,
  `student_name` varchar(255) default NULL,
  `mark` varchar(255) default '0',
  PRIMARY KEY (`id`)
) AUTO_INCREMENT=1;


DROP TABLE IF EXISTS `science`;

CREATE TABLE `science` (
  `id` mediumint(8) unsigned NOT NULL auto_increment,
  `student_name` varchar(255) default NULL,
  `mark` varchar(255) default '0',
  PRIMARY KEY (`id`)
) AUTO_INCREMENT=1;


DROP TABLE IF EXISTS `english`;

CREATE TABLE `english` (
  `id` mediumint(8) unsigned NOT NULL auto_increment,
  `student_name` varchar(255) default NULL,
  `mark` varchar(255) default '0',
  PRIMARY KEY (`id`)
) AUTO_INCREMENT=1;

DROP TABLE IF EXISTS `user_table`;

CREATE TABLE `user_table` (
  `user_id` mediumint(8) unsigned NOT NULL auto_increment,
  `login_email` varchar(255) default NULL,
  `name` varchar(255) default NULL,
  `password` varchar(255) default NULL,
  `user_type` varchar(255) default NULL,
  PRIMARY KEY (`user_id`)
) AUTO_INCREMENT=1;

INSERT INTO `user_table` (`name`)
VALUES
  ("arnold"),
  ("greg"),
  ("lewis");

