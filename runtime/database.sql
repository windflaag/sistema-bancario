/* SQL init file */

create table if not exists Accounts (
    accountId varchar(20) primary key,
    name varchar(80) not null,
    credit int default 0 check(credit >= 0),
    surname varcahr(80) not null
);

create table if not exists Transactions (
    transactionId varchar(36) primary key,
    fromId varchar(20),
    amount int check(amount > 0),
    toId varchar(20),
    timestamp timestamp not null,

    foreign key (fromID) references Accounts(accountId)
        on delete no action,
    foreign key (toID) references Accounts(accountId)
        on delete no action
);
