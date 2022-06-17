/* info board */

function createParagraph(text) {
    let paragraph = document.createElement("p");
    paragraph.innerText = text;
    return paragraph;
}

function createInfoCard(description, things) {
    let infoCard = document.createElement("div");
    infoCard.setAttribute("class", "infoCard");

    infoCard.appendChild(createParagraph(description));
    things.forEach((thing) => {
	infoCard.appendChild(createParagraph(thing));
    });
    
    return infoCard;
}

function createInfoBoard(identity, accountId, credit) {
    let infoBoard = document.createElement("div");
    infoBoard.setAttribute("class", "infoBoard");
    infoBoard.setAttribute("id", "theInfoBoard");

    infoBoard.appendChild(createInfoCard("Identity", [identity]));
    infoBoard.appendChild(createInfoCard("AccountID", [accountId]));
    infoBoard.appendChild(createInfoCard("Credit", [credit]));

    return infoBoard;
}

async function resetInfoBoard(identity, accountId, credit) {
    let oldInfoBoard = document.getElementById("theInfoBoard");
    let newInfoBoard = createInfoBoard(identity, accountId, credit);

    let gateway = document.getElementById("screen");
    gateway.replaceChild(newInfoBoard, oldInfoBoard);
}

/* transaction table */

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
    let placeholder = document.getElementById(transactionData.transactionId);
    ingress.replaceChild(tr, placeholder);
}

function createTransactionPlaceholder(transactionId) {
    let placeholder = document.createElement("div");
    placeholder.setAttribute("id", transactionId);
    return placeholder
}

function fetchTransactionData(transactionId, accountId) {
    // insert placeholder in order to preserve order
    let ingress = document.getElementById("theTransactionIngress");
    ingress.prepend(createTransactionPlaceholder(transactionId));

    $.ajax({
	method: "GET",
	url: `/api/transaction/${transactionId}`,
    })
	.fail(async (jqXHR, textStatus, errorThrown) => {
	    let placeholder = document.getElementById(transactionId); placeholder.remove();
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

/* fetch information for index */

async function fetchAccountDetail(accountId) {
    $.ajax({
	method: "GET",
	url: `/api/account/${accountId}`,
	async: true
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

/* transfer board */

function createTransferBoard(data, senderId, recipientId, amount) {
    let infoBoard = document.createElement("div");
    infoBoard.setAttribute("class", "transferBoard");
    infoBoard.setAttribute("id", "theTransferBoard");

    infoBoard.appendChild(createInfoCard("Sender", [senderId, data[senderId]]))
    infoBoard.appendChild(createInfoCard("Data", [data["transaction"], amount]));
    infoBoard.appendChild(createInfoCard("Recipient", [recipientId, data[recipientId]]));

    return infoBoard;
}

async function resetTransferBoard(data, senderId, recipientId, amount) {
    let oldTransferBoard = document.getElementById("theTransferBoard");
    let newTransferBoard = createTransferBoard(data, senderId, recipientId, amount);

    let gateway = document.getElementById("screen");
    gateway.replaceChild(newTransferBoard, oldTransferBoard);
}

/* enqueue transaction */

function enqueueTransaction(senderId, recipientId, amount) {
    let payload = {
	    from: senderId,
	    to: recipientId,
	    amount: amount
    };
    
    console.log(payload);
    $.ajax({
	url: "/api/transfer",
	data: JSON.stringify(payload),
	contentType: 'application/json',
	dataType: 'json',
	method: "POST",
	async: true
    })
	.fail((jqXHR, Code, Error) => {
	    alertUserThatHappened(`Bad Request: ${jqXHR.responseJSON.error}`);
	})
	.done((data) => {
	    resetTransferBoard(data, senderId, recipientId, amount);
	})
}

function transfer() {
    let senderId = document.getElementById("inputSenderAccountId").value;
    let recipientId = document.getElementById("inputRecipientAccountId").value;
    let amount = parseInt(document.getElementById("inputAmount").value);

    if (validateAccountId(senderId)) {
	if (validateAccountId(recipientId)) {
	    if ((!isNaN(amount)) && amount >= 0) {
		enqueueTransaction(senderId, recipientId, amount);
	    } else {
		alertUserThatHappened(`insert amount = "${amount}", is invalid`);
	    }
	} else {
	    alertUserThatHappened(`insert recipientID = "${recipientId}", is invalid`);
	}
    } else {
	alertUserThatHappened(`insert senderID = "${senderId}", is invalid`);
    }
}
