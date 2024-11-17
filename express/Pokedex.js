const express = require('express');
const path = require('path');
const axios = require('axios');

const app = express();
const port = 3000;

app.set('view engine', 'ejs');
app.set('views', path.join(__dirname));

app.use(express.json());

// Serve static files from the "icons" directory
app.use('/icons', express.static(path.join(__dirname, '../icons')));

// Serve the Index.html file
app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'Index.html'));
});

// Serve the Region.html file
app.get('/region/:region', async (req, res) => {
    const region = req.params.region;

    try {        
        // Send request to C++ backend (httplib) on port 8080
        const response = await axios.get(`http://localhost:8080/region/${region}`);

        // Get the data from the backend response
        const pokemonData = response.data;

        // Render Region.ejs
        res.render('Region', {
            region: region,
            pokemonData: pokemonData 
        });
    } catch (error) {
        console.error('Error fetching data from backend:', error);
        res.status(500).send('Internal Server Error');
    }
});

app.get('/pokemon/:pokedex_number/:region_id?', async (req, res) => {
    const pokedexNumber = req.params.pokedex_number;
    const regionId = req.params.region_id; // region_id is optional

    try {

    } catch (error) {
    console.error('Error fetching data from backend:', error);
    res.status(500).send('Internal Server Error');
    }
});

app.listen(port, () => {
    console.log(`Server running at http://localhost:${port}`);
});
