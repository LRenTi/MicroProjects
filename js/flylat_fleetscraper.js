// ==UserScript==
// @name         Flylat Fleet Scraper with Topbar, Health Indicators, and JSON Upload
// @namespace    http://tampermonkey.net/
// @version      0.7
// @description  Scrapes fleet data from Flylat, displays a topbar with timestamp, aircraft count, and health status indicators, allows JSON upload
// @author       You
// @match        https://app.flylat.net/fleet
// @grant        none
// ==/UserScript==

(function() {
    'use strict';

    if(localStorage.getItem("AIFleet")){
        localStorage.removeItem("AIFleet");
    }

    fetchAndMergeData();

    const topbar = document.createElement('div');
    topbar.id = 'customTopbar';
    topbar.classList.add('card');
    topbar.style.backgroundColor = '#4c8cb5';
    topbar.style.color = '#fff';
    topbar.style.padding = '10px';
    topbar.style.fontSize = '16px';
    topbar.style.marginBottom = '10px';
    topbar.style.position = 'relative';
    topbar.style.display = 'flex';
    topbar.style.flexDirection = 'row';

    const buttonarea = document.createElement('div');
    buttonarea.style.display = 'flex';
    buttonarea.style.gap = '5px';
    buttonarea.style.marginBottom = '10px';

    const aiButton = document.createElement('button');
    aiButton.textContent = 'Hire';
    aiButton.style.backgroundColor = '#fff';
    aiButton.style.color = '#4c8cb5';
    aiButton.style.border = '1px solid #4c8cb5';
    aiButton.style.padding = '5px 10px';
    aiButton.style.borderRadius = '5px';

    aiButton.onclick = function() {
        window.location.href = 'https://app.flylat.net/hire_ai';
    };

    buttonarea.appendChild(aiButton);

    const firstEmployeeCard = document.querySelector('.employee-card');
    if (firstEmployeeCard) {
        firstEmployeeCard.parentNode.insertBefore(topbar, firstEmployeeCard);
        firstEmployeeCard.parentNode.insertBefore(buttonarea, firstEmployeeCard);
    } else {
        console.error('No employee card found to insert the topbar.');
    }

    async function fetchAndMergeData() {
        try {
            // Daten von den zwei APIs abrufen
            const response1 = await fetch('https://lrenti.github.io/api/flylat/auto/airoutes.json');
            const response2 = await fetch('https://lrenti.github.io/api/flylat/auto/fleet.json');

            // Überprüfen, ob die Antworten erfolgreich waren
            if (!response1.ok || !response2.ok) {
                throw new Error('Error fetching data from APIs');
            }

            // JSON-Daten von beiden APIs konvertieren
            const data1 = await response1.json();
            const data2 = await response2.json();

            console.log("API 1 data:", data1);
            console.log("API 2 data:", data2);

            // Überprüfen, ob die Daten Arrays sind
            if (Array.isArray(data1) && Array.isArray(data2)) {
                // Holen der bestehenden fleetData
                const existingData = JSON.parse(localStorage.getItem('fleetData') || '[]');

                // Neue Daten aus beiden APIs zusammenführen
                const mergedData = [...data1, ...data2];

                // Verarbeiten der neuen Daten und Aktualisieren im localStorage
                mergedData.forEach(newAircraft => {
                    const existingAircraft = existingData.find(a => a.name === newAircraft.Name);
                    if (existingAircraft) {
                        // Aktualisieren der vorhandenen Daten
                        existingAircraft.hub = newAircraft.Hub || existingAircraft.hub; // Hub aktualisieren, falls vorhanden
                        existingAircraft.occupied = newAircraft.occupied || existingAircraft.occupied; // Occupied aktualisieren, falls vorhanden
                        existingAircraft.route = newAircraft.Route || existingAircraft.route; // Route hinzufügen oder aktualisieren
                        existingAircraft.departure = newAircraft.Departure || existingAircraft.departure; // Departure hinzufügen oder aktualisieren
                        existingAircraft.destination = newAircraft.Destination || existingAircraft.destination; // Destination hinzufügen oder aktualisieren
                    } else {
                        console.log("Aircraft not saved!");
                    }
                });

                // Speichern der aktualisierten Daten im localStorage
                localStorage.setItem('fleetData', JSON.stringify(existingData));

                updateTopbar();

            } else {
                console.error("Invalid data format: expected arrays from both APIs.");
            }
        } catch (err) {
            console.error("Error during fetch and merge:", err);
        }
    }

    function formatTimestamp() {
        const now = new Date();
        const day = String(now.getDate()).padStart(2, '0');
        const month = String(now.getMonth() + 1).padStart(2, '0');
        const year = String(now.getFullYear()).slice(2);
        const hours = String(now.getHours()).padStart(2, '0');
        const minutes = String(now.getMinutes()).padStart(2, '0');
        return `${day}.${month}.${year}, ${hours}:${minutes}`;
    }

    function updateTopbar() {
        let repairIdList = [];
        let transferList = [];
        let allTransferList = [];
        const fleetData = localStorage.getItem("fleetData");

        console.log("Loaded Fleet Data:", fleetData);
        const fleetArray = JSON.parse(fleetData);
        let repair = 0;
        let aiFlight = 0;
        let aiTransfer = 0;
        let allTransfer = 0;
        let aiReady = 0;

        console.log("Fleet:", fleetArray);

        if (fleetData) {
            // String zu einem Array parsen
            fleetArray.forEach(aircraft => {
                if (aircraft.status === "PARKED" && aircraft.health <= 50 /*&& aircraft.location === aircraft.hub*/) {
                    repair++;
                    repairIdList.push(aircraft.id);
                }
                if(aircraft.status === "AI IN FLIGHT"){
                    aiFlight++;
                }
                if(aircraft.status.includes("PARKED") && aircraft.route && ((aircraft.location !== aircraft.departure) || (aircraft.location !== aircraft.hub))){
                    aiTransfer++;
                    transferList.push(aircraft.name);
                }
                if(aircraft.status.includes("PARKED") && !aircraft.route && (aircraft.location !== aircraft.hub)){
                    allTransfer++;
                    allTransferList.push(aircraft.name + " - " + aircraft.hub);
                }
                if(aircraft.status.includes("PARKED") && aircraft.health > 50 && aircraft.route && aircraft.departure === aircraft.location){
                    aiReady++;
                }

            });
        }
        console.log("IDs to repair:", repairIdList);

        if (repairIdList.length > 0) {
            const repairAllButton = document.createElement('button');
            repairAllButton.textContent = 'Repair all';
            repairAllButton.style.backgroundColor = '#FF5733';
            repairAllButton.style.color = '#fff';
            repairAllButton.style.border = '1px solid #FF5733';
            repairAllButton.style.padding = '5px 10px';
            repairAllButton.style.borderRadius = '5px';

            repairAllButton.onclick = function() {
                const idsToSend = [...repairIdList];
                let requestsCompleted = 0;

                idsToSend.forEach((id, index) => {
                    const url = `https://app.flylat.net/fleet_ws/${id}`;
                    setTimeout(() => {
                        fetch(url, {
                            method: 'POST',
                        })
                            .then(response => {
                            requestsCompleted++;
                            if (response.ok) {
                                console.log("Formular erfolgreich abgeschickt für:", id);
                                repairIdList.splice(repairIdList.indexOf(id), 1);
                            } else {
                                console.error("Fehler beim Abschicken des Formulars für:", id, response.statusText);
                            }

                            // Seite neu laden, wenn alle Requests abgeschlossen sind
                            if (requestsCompleted === idsToSend.length) {
                                location.reload();
                            }
                        })
                            .catch(error => {
                            requestsCompleted++;
                            console.error("Netzwerkfehler für:", id, error);

                            // Seite neu laden, wenn alle Requests abgeschlossen sind
                            if (requestsCompleted === idsToSend.length) {
                                location.reload();
                            }
                        });
                    }, index * 25);
                });
            };
            buttonarea.appendChild(repairAllButton);
        }

        const topbar = document.getElementById('customTopbar');
        if (!topbar) return;

        // Clear previous content (except upload button)
        topbar.childNodes.forEach(child => {
            if (child.tagName !== 'LABEL') {
                child.remove();
            }
        });

        // Add timestamp (left-aligned)
        const timestamp = document.createElement('span');
        timestamp.style.marginRight = '10px';
        timestamp.textContent = formatTimestamp();
        topbar.appendChild(timestamp);

        if(repair >= 1){
            const repairAircraft = document.createElement('div');
            repairAircraft.style.display = 'flex';
            repairAircraft.style.alignItems = 'center';
            repairAircraft.style.justifyContent = 'center';
            repairAircraft.style.gap = '5px';
            repairAircraft.style.marginRight = '10px';
            const repairIcon = document.createElement('i');
            repairIcon.classList.add('fas', 'fa-screwdriver-wrench');
            repairIcon.style.fontSize = '20px';
            repairIcon.style.color = '#FF5733';
            const repairCount = document.createElement('span');
            repairCount.textContent = repair;
            repairCount.style.color = '#FF5733';
            repairAircraft.appendChild(repairIcon);
            repairAircraft.appendChild(repairCount);
            topbar.appendChild(repairAircraft);
        }

        // Create airplane icon and fleet count container
        const airplaneCount = document.createElement('div');
        airplaneCount.style.display = 'flex';
        airplaneCount.style.alignItems = 'center';
        airplaneCount.style.justifyContent = 'center';
        airplaneCount.style.gap = '5px';
        airplaneCount.style.marginRight = '10px';
        const airplane = document.createElement('i');
        airplane.classList.add('fas', 'fa-plane');
        airplane.style.fontSize = '20px';
        airplane.style.color = '#fff';
        const fleetCountSpan = document.createElement('span');
        fleetCountSpan.textContent = fleetArray.length;
        fleetCountSpan.style.color = '#fff';
        airplaneCount.appendChild(airplane);
        airplaneCount.appendChild(fleetCountSpan);
        topbar.appendChild(airplaneCount);

        console.log("Ai Flight", aiTransfer)

        if(aiFlight >= 1){
            const aiAircraft = document.createElement('div');
            aiAircraft.style.display = 'flex';
            aiAircraft.style.alignItems = 'center';
            aiAircraft.style.justifyContent = 'center';
            aiAircraft.style.gap = '5px';
            aiAircraft.style.marginRight = '10px';
            const aiIcon = document.createElement('i');
            aiIcon.classList.add('fas', 'fa-microchip');
            aiIcon.style.fontSize = '20px';
            aiIcon.style.color = '#fff';
            const aiCount = document.createElement('span');
            aiCount.textContent = aiFlight;
            aiCount.style.color = '#fff';
            aiAircraft.appendChild(aiIcon);
            aiAircraft.appendChild(aiCount);
            topbar.appendChild(aiAircraft);
        }

        if(aiTransfer >= 1){
            const aiTransferAircraft = document.createElement('div');
            aiTransferAircraft.style.display = 'flex';
            aiTransferAircraft.style.alignItems = 'center';
            aiTransferAircraft.style.justifyContent = 'center';
            aiTransferAircraft.style.gap = '5px';
            aiTransferAircraft.style.marginRight = '10px';
            const aiTransIcon = document.createElement('i');
            aiTransIcon.classList.add('fas', 'fa-right-left');
            aiTransIcon.style.fontSize = '20px';
            aiTransIcon.style.color = '#FF5733';
            const aiTransCount = document.createElement('span');
            aiTransCount.textContent = aiTransfer;
            aiTransCount.style.color = '#fff';
            aiTransferAircraft.appendChild(aiTransIcon);
            aiTransferAircraft.appendChild(aiTransCount);
            topbar.appendChild(aiTransferAircraft);

            // Liste erstellen und verstecken
            const transferListElement = document.createElement('ul');
            transferListElement.style.position = 'absolute';
            transferListElement.style.display = 'none';
            transferListElement.style.backgroundColor = '#041C3D';
            transferListElement.style.border = '1px solid #ccc';
            transferListElement.style.padding = '10px';
            transferListElement.style.listStyle = 'none';
            transferListElement.style.zIndex = '1000'; // Damit die Liste über dem restlichen Inhalt liegt

            // Namen zur Liste hinzufügen
            allTransferList.forEach(name => {
                const listItem = document.createElement('li');
                listItem.textContent = name;
                transferListElement.appendChild(listItem);
            });

            // Event-Listener für Hover hinzufügen
            aiTransferAircraft.addEventListener('mouseover', () => {
                transferListElement.style.display = 'block';
            });

            aiTransferAircraft.addEventListener('mouseout', () => {
                transferListElement.style.display = 'none';
            });

            // Die Liste als Kind von aiTransferAircraft hinzufügen
            aiTransferAircraft.appendChild(transferListElement);
        }

        if(allTransfer >= 1){
            const allTransferAircraft = document.createElement('div');
            allTransferAircraft.style.display = 'flex';
            allTransferAircraft.style.alignItems = 'center';
            allTransferAircraft.style.justifyContent = 'center';
            allTransferAircraft.style.gap = '5px';
            allTransferAircraft.style.marginRight = '10px';
            const allTransIcon = document.createElement('i');
            allTransIcon.classList.add('fas', 'fa-right-left');
            allTransIcon.style.fontSize = '20px';
            allTransIcon.style.color = '#fff';
            const allTransCount = document.createElement('span');
            allTransCount.textContent = allTransfer;
            allTransCount.style.color = '#fff';
            allTransferAircraft.appendChild(allTransIcon);
            allTransferAircraft.appendChild(allTransCount);
            topbar.appendChild(allTransferAircraft);

            // Liste erstellen und verstecken
            const allTransferListElement = document.createElement('ul');
            allTransferListElement.style.position = 'absolute';
            allTransferListElement.style.display = 'none';
            allTransferListElement.style.backgroundColor = '#041C3D';
            allTransferListElement.style.border = '1px solid #ccc';
            allTransferListElement.style.padding = '10px';
            allTransferListElement.style.listStyle = 'none';
            allTransferListElement.style.zIndex = '1000'; // Damit die Liste über dem restlichen Inhalt liegt

            // Namen zur Liste hinzufügen
            allTransferList.forEach(transfer => {
                const listItem = document.createElement('li');
                listItem.textContent = transfer;
                allTransferListElement.appendChild(listItem);
            });

            console.log("List: ", transferList);
            console.log(allTransferListElement);

            // Event-Listener für Hover hinzufügen
            allTransferAircraft.addEventListener('mouseover', () => {
                allTransferListElement.style.display = 'block';
            });

            allTransferAircraft.addEventListener('mouseout', () => {
                allTransferListElement.style.display = 'none';
            });

            allTransferAircraft.appendChild(allTransferListElement);
        }

        console.log("Ready to TakeOFF", aiReady)

        if(aiReady >= 1){
            const aiReadyAircraft = document.createElement('div');
            aiReadyAircraft.style.display = 'flex';
            aiReadyAircraft.style.alignItems = 'center';
            aiReadyAircraft.style.justifyContent = 'center';
            aiReadyAircraft.style.gap = '5px';
            aiReadyAircraft.style.marginRight = '10px';
            const aiReadyIcon = document.createElement('i');
            aiReadyIcon.classList.add('fas', 'fa-plane-departure');
            aiReadyIcon.style.fontSize = '20px';
            aiReadyIcon.style.color = '#4CBB17';
            const aiReadyCount = document.createElement('span');
            aiReadyCount.textContent = aiReady;
            aiReadyCount.style.color = '#fff';
            aiReadyAircraft.appendChild(aiReadyIcon);
            aiReadyAircraft.appendChild(aiReadyCount);
            topbar.appendChild(aiReadyAircraft);

            const autoHireButton = document.createElement('button');
            autoHireButton.textContent = 'Auto Hire';
            autoHireButton.style.backgroundColor = '#4CBB17';
            autoHireButton.style.color = '#fff';
            autoHireButton.style.border = '1px solid #4CBB17';
            autoHireButton.style.padding = '5px 10px';
            autoHireButton.style.borderRadius = '5px';

            autoHireButton.onclick = function() {
                autoHire();
            }

            buttonarea.appendChild(autoHireButton);
        }

    }

    function scrapeFleetData() {
        const fleetData = [];
        const aircraftCards = document.querySelectorAll('.employee-card');

        aircraftCards.forEach(card => {
            const nameElement = card.querySelector('h2');
            const aircraftElement = card.querySelector('p:nth-of-type(1)');
            const typeElement = card.querySelector('p:nth-of-type(2)');
            const statusElement = card.querySelector('p:nth-of-type(4)');
            const healthElement = card.querySelector('.progress-bar');

            const workshopButtons = card.querySelectorAll('button[title="Transfer Aircraft"]'); // Buttons nur innerhalb der aktuellen Karte suchen
            let id = null;

            workshopButtons.forEach(button => {
                const onclickAttr = button.getAttribute('onclick'); // Get the onclick attribute
                const idMatch = onclickAttr.match(/\/(\d+)'/); // Extract the ID using regex

                if (idMatch) {
                    id = idMatch[1]; // Get the ID (first capturing group)
                    console.log("Extracted ID:", id);
                }
            });

            if (nameElement && aircraftElement && typeElement && statusElement && healthElement) {
                const name = nameElement.innerText;
                const aircraft = aircraftElement.innerText;
                const type = typeElement.innerText.replace("Type: ", "").trim();
                const health = parseInt(healthElement.innerText.trim());

                let status, location;
                if (statusElement.innerText.includes("AI IN FLIGHT")) {
                    status = "AI IN FLIGHT";
                    location = null;
                } else if (statusElement.innerText.includes("Transferring")) {
                    status = "TRANSFERRING";
                    location = null;
                } else if (statusElement.innerText.includes("WORKSHOP")) {
                    status = "WORKSHOP";
                    const locationMatch = statusElement.innerText.match(/\(([^)]+)\)/);
                    location = locationMatch ? locationMatch[1] : null;
                } else {
                    status = statusElement.innerText.trim().split(' ')[0];
                    const locationMatch = statusElement.innerText.match(/\(([^)]+)\)/);
                    location = locationMatch ? locationMatch[1] : null;
                }

                fleetData.push({
                    id: id,
                    name: name,
                    aircraft: aircraft,
                    type: type,
                    status: status,
                    location: location,
                    health: health,
                    hub: null,
                    occupied: null,
                    route: null,
                    departure: null,
                    destination: null,
                });
            }
        });

        if (fleetData.length > 0) {
            localStorage.setItem('fleetData', JSON.stringify(fleetData));
            localStorage.setItem('timeStamp', new Date().toISOString());
            console.log("Fleet data saved successfully!", fleetData);
        } else {
            console.log("No valid fleet data found.");
        }
    }

    function autoHire() {
        const fleetData = JSON.parse(localStorage.getItem('fleetData') || '[]');
        if (fleetData.length === 0) {
            console.log("No fleetData in localStorage.");
            return;
        }

        const availableAircraft = fleetData.filter(aircraft =>
                                                   aircraft.occupied == "AI" &&
                                                   aircraft.status == "PARKED" &&
                                                   aircraft.hub == aircraft.location
                                                  );

        console.log("Available Aircraft:", availableAircraft);

        localStorage.removeItem('AIFleet');
        localStorage.setItem('AIFleet', JSON.stringify(availableAircraft));
        localStorage.removeItem('timeStamp');
        localStorage.removeItem('fleetData');

        if (availableAircraft.length === 0) {
            localStorage.removeItem('AIFleet');
            return;
        }

        const fleetDataForSubmit = JSON.parse(localStorage.getItem("AIFleet") || "[]");

        if (fleetDataForSubmit.length === 0) {
            console.warn("Keine verfügbaren Flugdaten im Speicher.");
            return;
        }

        // Versprechen für jede Anfrage sammeln
        const submitPromises = fleetDataForSubmit.map(aircraft => {
            const data = {
                crewName: aircraft.route,
                departure: aircraft.departure,
                destination: aircraft.destination,
                aircraft: aircraft.id
            };

            const formData = new FormData();
            formData.append("crewName", data.crewName);
            formData.append("departure", data.departure);
            formData.append("destination", data.destination);
            formData.append("aircraft", data.aircraft);

            // Fetch-Anfrage erstellen
            return fetch('https://app.flylat.net/employees/create_crew.php', {
                method: 'POST',
                body: formData
            })
                .then(response => {
                if (!response.ok) {
                    throw new Error(`Fehler für ${data.aircraft}: ${response.statusText}`);
                }
                console.log("Formular erfolgreich abgeschickt für:", data);
                removeAircraftFromStorage(aircraft.name);
            })
                .catch(error => {
                console.error("Netzwerkfehler für:", data, error);
                throw error; // Fehler weitergeben
            });
        });

        // Alle Promises abwarten
        Promise.all(submitPromises)
            .then(() => {
            console.log("Alle Formulare erfolgreich abgeschickt.");
            window.location.reload(); // Seite neu laden
        })
            .catch(error => {
            console.error("Mindestens ein Formular konnte nicht abgeschickt werden.", error);
            alert("Es gab einen Fehler beim Abschicken einiger Formulare. Bitte prüfen Sie die Konsole für Details.");
        });
    }
    function removeAircraftFromStorage(selectedAircraftName) {
        const fleetData = JSON.parse(localStorage.getItem('AIFleet') || '[]');
        if (fleetData.length === 0) return;

        const updatedFleetData = fleetData.filter(aircraft => aircraft.name !== selectedAircraftName);
        localStorage.setItem('AIFleet', JSON.stringify(updatedFleetData));
        console.log("Updated AIFleet after removal:", updatedFleetData);
    }


    function init() {
        scrapeFleetData();
    }

    scrapeFleetData();


    window.addEventListener('load', init);

})();
