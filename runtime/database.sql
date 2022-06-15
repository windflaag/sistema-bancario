/* SQL init file */

create table if not exists Accounts (
    accountId varchar(20) primary key,
    name varchar(80) not null,
    credit int default 0 check(credit >= 0),
    surname varcahr(80) not null
);

create table if not exists Transactions (
    transactionId varchar(80) primary key,
    fromID varchar(20) not null,
    amount int check(amount > 0),
    toID varchar(20) not null,
    timestamp timestamp not null,

    foreign key (fromID) references Accounts(accountId)
        on delete no action,
    foreign key (toID) references Accounts(accountId)
        on delete no action
);
