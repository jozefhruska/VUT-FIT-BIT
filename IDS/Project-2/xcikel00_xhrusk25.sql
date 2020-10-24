-- Clean
---------------------
DROP TABLE "AIRCRAFT" CASCADE CONSTRAINTS;
DROP TABLE "AIRCRAFT_TYPE" CASCADE CONSTRAINTS;
DROP TABLE "AIRLINE" CASCADE CONSTRAINTS;
DROP TABLE "AIRPORT" CASCADE CONSTRAINTS;
DROP TABLE "BOARDING_TICKET" CASCADE CONSTRAINTS;
DROP TABLE "EMPLOYEE" CASCADE CONSTRAINTS;
DROP TABLE "FLIGHT" CASCADE CONSTRAINTS;
DROP TABLE "GATE" CASCADE CONSTRAINTS;
DROP TABLE "GATE_COMPATIBILITY" CASCADE CONSTRAINTS;
DROP TABLE "PERSON" CASCADE CONSTRAINTS;
DROP TABLE "SEAT" CASCADE CONSTRAINTS;
DROP TABLE "TICKET" CASCADE CONSTRAINTS;

-- Tables
---------------------
create table "AIRLINE" (
    "AIRLINE_ID" number(6, 0) generated by default as identity not null,
    "NAME" varchar2(60) not null,
    "COUNTRY" varchar2(80),
    primary key ("AIRLINE_ID")
);

create table "AIRPORT" (
    "AIRPORT_ID" varchar2(3) not null CHECK(REGEXP_LIKE(AIRPORT_ID, '[A-Z][A-Z][A-Z]')),
    "NAME" varchar2(80) not null,
    "ADDRESS" varchar2(120),
    "COUNTRY" varchar2(80),
    primary key ("AIRPORT_ID")
    
);

create table "GATE" (
    "GATE_ID" varchar2(10) not null,
    "TERMINAL" varchar2(10) not null,
    primary key ("GATE_ID")
);

create table "FLIGHT" (
    "FLIGHT_ID" number(10, 0) generated by default as identity not null,
    "DEPARTURE_TIME" timestamp not null,
    "ARRIVAL_TIME" timestamp not null,
    "AIRLINE" number(6, 0) not null,
    "DESTINATION" varchar2(3) not null,
    "GATE" varchar2(3) not null,
    primary key ("FLIGHT_ID")
);

create table "GATE_COMPATIBILITY" (
    "GATE" varchar2(10) not null,
    "AIRCRAFT_TYPE" varchar2(4),
    primary key ("GATE", "AIRCRAFT_TYPE")
);

create table "AIRCRAFT_TYPE" (
    "AIRCRAFT_TYPE_ID" varchar2(4) not null CHECK(REGEXP_LIKE(AIRCRAFT_TYPE_ID, '[A-Z]...')),
    "MANUFACTURER" varchar2(100) not null,
    primary key ("AIRCRAFT_TYPE_ID")
);

create table "AIRCRAFT" (
    "AIRCRAFT_ID" number(10, 0) generated by default as identity not null,
    "MANUFACTURING_DATE" date not null,
    "REVISION_DATE" date,
    "CREW_NUM" number(2, 0) not null,
    "AIRCRAFT_TYPE" varchar2(4) not null,
    primary key ("AIRCRAFT_ID")
);

create table "SEAT" (
    "AIRCRAFT_ID" number(10, 0),
    "SEAT_NUMBER" number(4, 0),
    "CLASS" varchar2(20),
    "POSITION" varchar2(20),
    primary key ("AIRCRAFT_ID", "SEAT_NUMBER")
);

create table "PERSON" (
    "PASSPORT_ID" varchar2(20) not null,
    "FIRST_NAME" varchar2(100) not null,
    "LAST_NAME" varchar2(100) not null,
    "ADDRESS" varchar2(120) not null,
    "NATIONALITY" varchar2(80) not null,
    "EMPLOYEE_ID" number(10, 0),
    primary key ("PASSPORT_ID")
);

create table "EMPLOYEE" (
    "EMPLOYEE_ID" number(10, 0) generated by default as identity not null,
    "POSITION" varchar2(60) not null,
    primary key ("EMPLOYEE_ID")
);

create table "TICKET" (
    "TICKET_ID" number(10, 0) generated by default as identity not null,
    "FLIGHT_ID" number(10, 0) not null,
    "PASSPORT_ID" varchar2(20) not null,
     primary key ("TICKET_ID")
);

create table "BOARDING_TICKET" (
    "TICKET_ID" number(10, 0) not null,
    "AIRCRAFT_ID" number(10, 0) not null,
    "SEAT_NUMBER" number(4, 0) not null,
    "BOARDING_TIME" timestamp not null,
    primary key ("TICKET_ID", "BOARDING_TIME")
);

-- Constraints
---------------------
alter table "FLIGHT"
    add constraint "FLIGHT_AIRLINE_FK"
    foreign key ("AIRLINE")
    references "AIRLINE";

alter table "FLIGHT"
    add constraint "FLIGHT_DESTINATION_FK"
    foreign key ("DESTINATION")
    references "AIRPORT";

alter table "FLIGHT"
    add constraint "FLIGHT_GATE_FK"
    foreign key ("GATE")
    references "GATE";
    
alter table "GATE_COMPATIBILITY"
    add constraint "GATE_COMPATIBILITY_GATE_FK"
    foreign key ("GATE")
    references "GATE";
    
alter table "GATE_COMPATIBILITY"
    add constraint "GATE_COMPATIBILITY_AIRCRAFT_TYPE_FK"
    foreign key ("AIRCRAFT_TYPE")
    references "AIRCRAFT_TYPE";

alter table "AIRCRAFT"
    add constraint "AIRCRAFT_TYPE_FK"
    foreign key ("AIRCRAFT_TYPE")
    references "AIRCRAFT_TYPE";
    
alter table "PERSON"
    add constraint "PERSON_EMPLOYEE_FK"
    foreign key ("EMPLOYEE_ID")
    references "EMPLOYEE";
    
alter table "TICKET"
    add constraint "TICKET_FLIGHT_FK"
    foreign key ("FLIGHT_ID")
    references "FLIGHT";
    
alter table "TICKET"
    add constraint "TICKET_PASSPORT_FK"
    foreign key ("PASSPORT_ID")
    references "PERSON";
    
alter table "SEAT"
    add constraint "SEAT_AIRCRAFT_FK"
    foreign key ("AIRCRAFT_ID")
    references "AIRCRAFT";
    
alter table "BOARDING_TICKET"
    add constraint "BOARDING_TICKET_SEAT_FK"
    foreign key ("AIRCRAFT_ID", "SEAT_NUMBER")
    references SEAT("AIRCRAFT_ID", "SEAT_NUMBER");


-- Sample data
---------------------    
insert into "AIRLINE" ("NAME", "COUNTRY") VALUES ('FIT Airlines', 'Czech Republic');
insert into "AIRLINE" ("NAME", "COUNTRY") VALUES ('FIT Wings', 'Slovakia');

insert into "AIRPORT" ("AIRPORT_ID", "NAME", "ADDRESS", "COUNTRY") VALUES ('LHR', 'Heathrow Airport', 'Longford TW6', 'UK');
insert into "AIRPORT" ("AIRPORT_ID", "NAME", "ADDRESS", "COUNTRY") VALUES ('JFK', 'John F. Kennedy International Airport', 'Queens, NY 11430', 'USA');

insert into "AIRCRAFT_TYPE" ("AIRCRAFT_TYPE_ID", "MANUFACTURER") VALUES ('B733', 'Awesome planes');
insert into "AIRCRAFT_TYPE" ("AIRCRAFT_TYPE_ID", "MANUFACTURER") VALUES ('B734', 'Awesome planes');

insert into "AIRCRAFT" ("MANUFACTURING_DATE", "REVISION_DATE", "CREW_NUM", "AIRCRAFT_TYPE") VALUES (
    to_date('2011-02-1','yyyy-MM-dd'),
    to_date('2018-11-1','yyyy-MM-dd'),
    6,
    'B733'
);

insert into "SEAT" ("AIRCRAFT_ID", "SEAT_NUMBER", "CLASS", "POSITION") VALUES (
    1,
    10,
    'Business',
    'Window'
);

insert into "GATE" ("GATE_ID", "TERMINAL") VALUES ('17', '12');
insert into "GATE" ("GATE_ID", "TERMINAL") VALUES ('18A', '12');

insert into "FLIGHT" ("DEPARTURE_TIME", "ARRIVAL_TIME", "AIRLINE", "DESTINATION", "GATE") VALUES (
    to_date('2019-03-22 8:00','yyyy-MM-dd HH24:MI'),
    to_date('2019-03-22 15:00','yyyy-MM-dd HH24:MI'),
    1,
    'JFK',
    '18A'
);

insert into "FLIGHT" ("DEPARTURE_TIME", "ARRIVAL_TIME", "AIRLINE", "DESTINATION", "GATE") VALUES (
    to_date('2019-03-23 11:00','yyyy-MM-dd HH24:MI'),
    to_date('2019-03-24 7:00','yyyy-MM-dd HH24:MI'),
    2,
    'LHR',
    '17'
);

insert into "EMPLOYEE" ("POSITION") VALUES (
    'Cleaning lady'
);

insert into "PERSON" ("PASSPORT_ID", "FIRST_NAME", "LAST_NAME", "ADDRESS", "NATIONALITY", "EMPLOYEE_ID") VALUES (
    '3112313441',
    'Tomáš',
    'Čikel',
    'Purkyňova 93',
    'Czechia',
    1
);

insert into "PERSON" ("PASSPORT_ID", "FIRST_NAME", "LAST_NAME", "ADDRESS", "NATIONALITY") VALUES (
    '3112313442',
    'Jozef',
    'Hruška',
    'Purkyňova 93',
    'Czechia'
);

insert into "TICKET" ("FLIGHT_ID", "PASSPORT_ID") VALUES (
    1,
    '3112313442'
);

insert into "BOARDING_TICKET" ("TICKET_ID", "AIRCRAFT_ID", "SEAT_NUMBER", "BOARDING_TIME") VALUES (
    1,
    1,
    10,
    to_date('2019-03-23 11:00','yyyy-MM-dd HH24:MI')
);