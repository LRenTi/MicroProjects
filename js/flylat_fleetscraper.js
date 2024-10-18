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

    fetchAndMergeData();

    // Create the topbar element
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
    topbar.style.flexDirection = 'row'; // Stack elements vertically if needed

    // Create and style the right-aligned "AI" button
    const buttonarea = document.createElement('div');
    buttonarea.style.marginBottom = '10px';

    const aiButton = document.createElement('button');
    aiButton.textContent = 'to AI';
    aiButton.style.backgroundColor = '#fff';
    aiButton.style.color = '#4c8cb5';
    aiButton.style.border = '1px solid #4c8cb5';
    aiButton.style.padding = '5px 10px';
    aiButton.style.borderRadius = '5px';

    // Button opens a new window on click
    aiButton.onclick = function() {
        window.location.href = 'https://app.flylat.net/fleet';
    };

    buttonarea.appendChild(aiButton);

    // Insert the topbar before the first employee-card
    const firstEmployeeCard = document.querySelector('.employee-card');
    if (firstEmployeeCard) {
        firstEmployeeCard.parentNode.insertBefore(topbar, firstEmployeeCard);
        firstEmployeeCard.parentNode.insertBefore(buttonarea, firstEmployeeCard);
    } else {
        console.error('No employee card found to insert the topbar.');
    }

    // Handle file upload
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
                        // Hinzufügen neuer Flugzeuge
                        existingData.push({
                            name: newAircraft.Name,
                            hub: newAircraft.Hub,
                            occupied: newAircraft.occupied,
                            health: 100, // Beispielwert; kannst du anpassen
                            route: newAircraft.Route || null, // Route hinzufügen
                            departure: newAircraft.Departure || null, // Departure hinzufügen
                            destination: newAircraft.Destination || null // Destination hinzufügen
                        });
                    }
                });

                // Speichern der aktualisierten Daten im localStorage
                localStorage.setItem('fleetData', JSON.stringify(existingData));

            } else {
                console.error("Invalid data format: expected arrays from both APIs.");
            }
        } catch (err) {
            console.error("Error during fetch and merge:", err);
        }
    }

    // Format the timestamp as dd-mm-yy hh:mm
    function formatTimestamp() {
        const now = new Date();
        const day = String(now.getDate()).padStart(2, '0');
        const month = String(now.getMonth() + 1).padStart(2, '0');
        const year = String(now.getFullYear()).slice(2);
        const hours = String(now.getHours()).padStart(2, '0');
        const minutes = String(now.getMinutes()).padStart(2, '0');
        return `${day}.${month}.${year}, ${hours}:${minutes}`;
    }

    // Create and append the timestamp, airplane icon, and colored circles with aircraft counts
    function updateTopbar(fleetCount, healthyCount, lowHealthCount) {
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
        timestamp.style.marginRight = '20px';
        timestamp.textContent = formatTimestamp();
        topbar.appendChild(timestamp);

        // Create airplane icon and fleet count container
        const airplaneCount = document.createElement('div');
        airplaneCount.style.display = 'flex';
        airplaneCount.style.alignItems = 'center';
        airplaneCount.style.justifyContent = 'center';
        airplaneCount.style.gap = '5px';
        const airplane = document.createElement('i');
        airplane.classList.add('fas', 'fa-plane');
        airplane.style.fontSize = '20px';
        airplane.style.color = '#fff';
        const fleetCountSpan = document.createElement('span');
        fleetCountSpan.textContent = fleetCount;
        fleetCountSpan.style.color = '#fff';
        airplaneCount.appendChild(airplane);
        airplaneCount.appendChild(fleetCountSpan);
        topbar.appendChild(airplaneCount);

        // Add green circle for healthy aircraft (Health > 50%)
        const greenCircle = document.createElement('span');
        greenCircle.style.display = 'inline-block';
        greenCircle.style.width = '20px';
        greenCircle.style.height = '20px';
        greenCircle.style.backgroundColor = 'green';
        greenCircle.style.borderRadius = '50%';
        greenCircle.style.margin = '0 10px';
        topbar.appendChild(greenCircle);

        const healthyCountSpan = document.createElement('span');
        healthyCountSpan.textContent = healthyCount;
        topbar.appendChild(healthyCountSpan);

        // Add red circle for low health aircraft (Health <= 50%)
        const redCircle = document.createElement('span');
        redCircle.style.display = 'inline-block';
        redCircle.style.width = '20px';
        redCircle.style.height = '20px';
        redCircle.style.backgroundColor = 'red';
        redCircle.style.borderRadius = '50%';
        redCircle.style.marginLeft = '10px';
        topbar.appendChild(redCircle);

        const lowHealthCountSpan = document.createElement('span');
        lowHealthCountSpan.textContent = lowHealthCount;
        topbar.appendChild(lowHealthCountSpan);
    }

    // Scrape fleet data
    function scrapeFleetData() {
        const fleetData = [];
        let healthyCount = 0;
        let lowHealthCount = 0;
        const aircraftCards = document.querySelectorAll('.employee-card');

        aircraftCards.forEach(card => {
            const nameElement = card.querySelector('h2');
            const aircraftElement = card.querySelector('p:nth-of-type(1)');
            const typeElement = card.querySelector('p:nth-of-type(2)');
            const statusElement = card.querySelector('p:nth-of-type(4)');
            const healthElement = card.querySelector('.progress-bar');

            const allForms = card.querySelectorAll('form');

            allForms.forEach(formElement => {
                formElement.addEventListener('submit', function(event) {
                    event.preventDefault(); // Prevent the default form submission
                    const actionUrl = formElement.action; // Get the action URL
                    window.open(actionUrl, '_blank'); // Open the URL in a new tab
                });
            });

            if (nameElement && aircraftElement && typeElement && statusElement && healthElement) {
                const name = nameElement.innerText;
                const aircraft = aircraftElement.innerText;
                const type = typeElement.innerText.replace("Type: ", "").trim();
                const health = parseInt(healthElement.innerText.trim());

                // Increment health-based counters
                if (health > 50) {
                    healthyCount++;
                } else {
                    lowHealthCount++;
                }

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
                    name: name,
                    aircraft: aircraft,
                    type: type,
                    status: status,
                    location: location,
                    health: health,
                });
            }
        });

        if (fleetData.length > 0) {
            localStorage.setItem('fleetData', JSON.stringify(fleetData));
            localStorage.setItem('timeStamp', new Date().toISOString());
            console.log("Fleet data saved successfully!", fleetData);
            updateTopbar(fleetData.length, healthyCount, lowHealthCount); // Update topbar with health counts
        } else {
            console.log("No valid fleet data found.");
        }
    }

    // Initialize the script
    function init() {
        scrapeFleetData();
    }

    scrapeFleetData();

    // Ensure the window is fully loaded before running the script
    window.addEventListener('load', init);

})();
