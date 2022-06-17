/* info board */

function createParagraph(text) {
    let paragraph = document.createElement("p");
    paragraph.innerText = text;
    return paragraph;
}

function createInfoCard(description, thing) {
    let infoCard = document.createElement("div");
    infoCard.setAttribute("class", "infoCard");

    infoCard.appendChild(createParagraph(description));
    infoCard.appendChild(createParagraph(thing));
    return infoCard;
}

function createInfoBoard(identity, accountId, credit) {
    let infoBoard = document.createElement("div");
    infoBoard.setAttribute("class", "infoBoard");
    infoBoard.setAttribute("id", "theInfoBoard");

    infoBoard.appendChild(createInfoCard("Identity", identity));
    infoBoard.appendChild(createInfoCard("AccountID", accountId));
    infoBoard.appendChild(createInfoCard("Credit", credit));

    return infoBoard;
}

async function resetInfoBoard(identity, accountId, credit) {
    let oldInfoBoard = document.getElementById("theInfoBoard");
    let newInfoBoard = createInfoBoard(identity, accountId, credit);

    let gateway = document.getElementById("screen");
    gateway.replaceChild(newInfoBoard, oldInfoBoard);
}

function createTableHeader(header) {
    let th = document.createElement("th");
    th.innerText = header;
    return th;
}

function createTransactionTableHeader() {
    let thead = document.createElement("thead");

    thead.appendChild(createTableHeader("transactionID"));
    thead.appendChild(createTableHeader("recipient"));
    thead.appendChild(createTableHeader("timestamp"));
    thead.appendChild(createTableHeader("amount"));
    
    return thead;
}

function createTransactionTableBody() {
    let tbody = document.createElement("tbody");
    tbody.setAttribute("id", "theTransactionIngress");

    return tbody;
}

function createTransactionTable() {
    let transactionTable = document.createElement("table");
    transactionTable.setAttribute("class", "transactionTable");
    transactionTable.setAttribute("id", "theTransactionTable");

    transactionTable.appendChild(createTransactionTableHeader());
    transactionTable.appendChild(createTransactionTableBody());

    return transactionTable;
}

function assignClass(object, class_) {
    object.setAttribute("class", class_);
    return object;
}

function createTableData(field) {
    let td = document.createElement("td");
    td.innerText = field;
    return td;
}

function createTableRow(data, accountId) {
    let tr = document.createElement("tr");

    tr.append(createTableData(data.transactionId));

    if (accountId != data.recipient) {
	tr.append(createTableData(data.recipient));
	data.amount = - data.amount;
    } else {
	tr.append(createTableData(""));
    }

    tr.append(createTableData(data.timestamp));

    if (data.amount > 0) {
	tr.append(assignClass(createTableData(data.amount), "deposit"));
    } else if (data.amount < 0) {
	tr.append(assignClass(createTableData(data.amount), "preliev"));
    } else {
	tr.append(createTableData(data.amount));
    }
    
    return tr;
}

function populateTransactionTable(transactionData, accountId) {
    let tr = createTableRow(transactionData, accountId);
    let ingress = document.getElementById("theTransactionIngress");
    ingress.prepend(tr);
}

function fetchTransactionData(transactionId, accountId) {
    $.ajax({
	method: "GET",
	url: `/api/transaction/${transactionId}`
    })
	.fail(async (jqXHR, textStatus, errorThrown) => {
	    alertUserThatHappened(`unable to fetch data for transactionId = ${transactionId}`);
	})
	.done(async (data) => {
	    populateTransactionTable(data, accountId)
	});
}

function schedulePopulation(transactionIDs, accountId) {
    transactionIDs.forEach((transactionId) => {
	fetchTransactionData(transactionId, accountId)
    });
}

async function resetTransactionTable(transactionIDs, accountId) {
    let oldTransactionTable = document.getElementById("theTransactionTable");
    let newTransactionTable = createTransactionTable();

    let gateway = document.getElementById("screen");
    gateway.replaceChild(newTransactionTable, oldTransactionTable);
    schedulePopulation(transactionIDs, accountId);
}

async function fetchAccountDetail(accountId) {
    $.ajax({
	method: "GET",
	url: `/api/account/${accountId}`
    })
	.fail(async (jqXHR, textStatus, errorThrown) => {
	    alertUserThatHappened(`unable to fetch data for accountID = ${accountId}`);
	})
	.done(async (data) => {
	    populatePage(data, accountId)
	});
}

function populatePage(data, accountId) {
    resetInfoBoard(`${data["name"]} ${data["surname"]}`, accountId, data["credit"])
    resetTransactionTable(data["transactions"], accountId);
}

function invalidAccountIdChar(c) {
    if ("0123456789abcdef".includes(c))
	return false;
    return true;
}

function validateAccountId(accountId) {
    if (accountId.length < 20)
	return false;
    for (let index in accountId)
	if (invalidAccountIdChar(accountId[index]))
	    return false;
    return true;
}

function loadPage() {
    let accountId = document.getElementById("inputAccountId").value;
    if (validateAccountId(accountId))
	fetchAccountDetail(accountId);
    else
	alertUserThatHappened(`insert accountID = "${accountId}", is invalid`);
	    
}

function alertUserThatHappened(something) {
    var banner = document.getElementById("banner");

    banner.innerText = something;
    banner.className = "show";

    setTimeout(function(){ banner.className = banner.className.replace("show", ""); }, 3000);
}
