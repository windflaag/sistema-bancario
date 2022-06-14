/* SQL init file */

create table if not exists Account (
    accountId varchar(20) primary key,
    name varchar(80) not null,
    surname varcahr(80) not null
);
