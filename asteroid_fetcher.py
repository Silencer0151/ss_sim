#!/usr/bin/env python3
"""
Fetch asteroid orbital data from JPL Horizons for C++ Solar System Simulator
This script uses the Horizons API to get position and velocity vectors
"""

import requests
import re
from datetime import datetime

# Configuration
EPOCH = "2025-Aug-17 00:00"  # Match your existing planet data
CENTER = "@sun"  # Heliocentric coordinates

# List of notable asteroids to fetch (you can modify this list)
# Format: (IAU_number, name, color_rgb)
ASTEROIDS = [
    # Test with a known working asteroid first
    (451, "Patientia", "(150, 150, 150)"),
    
    # Large main belt asteroids (excluding ones already in your code)
    (511, "Davida", "(160, 160, 160)"),
    (52, "Europa", "(170, 170, 170)"),
    
    # If these work, uncomment the rest:
    # (87, "Sylvia", "(150, 150, 150)"),
    # (15, "Eunomia", "(160, 160, 160)"),
    # (3, "Juno", "(190, 190, 190)"),
    # (16, "Psyche", "(180, 150, 120)"),  # Metallic asteroid
    # (31, "Euphrosyne", "(140, 140, 140)"),
    
    # # Near-Earth asteroids
    # (433, "Eros", "(200, 180, 160)"),
    # (25143, "Itokawa", "(180, 180, 180)"),
    # (101955, "Bennu", "(100, 100, 100)"),
    # (162173, "Ryugu", "(80, 80, 80)"),
    # (99942, "Apophis", "(200, 100, 100)"),  # Potentially hazardous
]

def fetch_horizons_data(object_id, object_name):
    """Fetch vector data from JPL Horizons web API"""
    
    url = "https://ssd.jpl.nasa.gov/api/horizons.api"
    
    # For numbered asteroids, just use the number with semicolon
    params = {
        'format': 'text',
        'COMMAND': f'{object_id};',  # Just the number with semicolon
        'EPHEM_TYPE': 'VECTORS',
        'CENTER': '500@10',  # Sun center
        'START_TIME': '2025-Aug-17',  # Simple date format without time
        'STOP_TIME': '2025-Aug-18',    # Simple date format without time
        'STEP_SIZE': '1d',
        'OUT_UNITS': 'KM-S',  # km and km/s
        'REF_PLANE': 'ECLIPTIC',
        'REF_SYSTEM': 'ICRF',
        'VEC_LABELS': 'YES',
        'VEC_DELTA_T': 'NO',
        'OBJ_DATA': 'YES',
        'VEC_TABLE': '2',  # State vectors (position and velocity)
    }
    
    # Debug: Show the actual request being made
    print(f"  → Requesting asteroid {object_id}")
    
    try:
        response = requests.get(url, params=params)
        if response.status_code == 200:
            result = parse_horizons_output(response.text, object_name)
            if result:
                print(f"  ✓ Got data for {object_name}")
            return result
        else:
            print(f"Error fetching {object_name}: HTTP {response.status_code}")
            return None
    except Exception as e:
        print(f"Error fetching {object_name}: {e}")
        return None

def parse_horizons_output(text, object_name):
    """Parse the Horizons output to extract position, velocity, and physical data"""
    
    data = {}
    
    # Extract mass (GM parameter) - look for various formats
    gm_patterns = [
        r'GM.*?=\s*([\d.E+-]+)\s*(?:km\^3/s\^2)?',  # Standard format
        r'GM\s*=\s*([\d.E+-]+)',  # Simple format
        r'GM,?\s*km\^3/s\^2\s*=\s*([\d.E+-]+)',  # Alternative format
    ]
    
    gm_found = False
    for pattern in gm_patterns:
        gm_match = re.search(pattern, text, re.IGNORECASE)
        if gm_match:
            try:
                gm = float(gm_match.group(1))
                # Convert GM to mass: M = GM/G where G = 6.67430e-20 km^3/(kg*s^2)
                data['mass'] = gm / 6.67430e-20
                gm_found = True
                break
            except:
                continue
    
    if not gm_found:
        # Estimate mass based on radius if GM not available
        data['mass'] = 1e18  # Default small mass
    
    # Extract radius - look for various formats
    radius_patterns = [
        r'Radius.*?\(km\)\s*=\s*([\d.E+-]+)',
        r'RAD\s*=\s*([\d.E+-]+)',
        r'Radius.*?=\s*([\d.E+-]+)\s*km',
        r'Mean radius.*?=\s*([\d.E+-]+)',
    ]
    
    radius_found = False
    for pattern in radius_patterns:
        radius_match = re.search(pattern, text, re.IGNORECASE)
        if radius_match:
            try:
                data['radius'] = float(radius_match.group(1)) * 1000  # Convert to meters
                radius_found = True
                break
            except:
                continue
    
    if not radius_found:
        data['radius'] = 10000  # Default 10 km radius
    
    # Extract position and velocity vectors from the data section
    # Look for the section between $$SOE and $$EOE
    soe_match = re.search(r'\$\$SOE(.*?)\$\$EOE', text, re.DOTALL)
    if soe_match:
        vector_section = soe_match.group(1).strip()
        lines = vector_section.split('\n')
        
        # Parse the vector data - it should be in a specific format
        # Look for lines with X, Y, Z and VX, VY, VZ
        position_found = False
        velocity_found = False
        
        for i, line in enumerate(lines):
            # Position line (X, Y, Z)
            if 'X =' in line or 'X=' in line:
                # Try different formats
                patterns = [
                    r'X\s*=\s*([-\d.E+-]+)\s+Y\s*=\s*([-\d.E+-]+)\s+Z\s*=\s*([-\d.E+-]+)',
                    r'([-\d.E+-]+)\s+([-\d.E+-]+)\s+([-\d.E+-]+)',  # Just numbers
                ]
                for pattern in patterns:
                    match = re.search(pattern, line)
                    if match:
                        try:
                            data['position'] = [
                                float(match.group(1)) * 1000,  # Convert km to m
                                float(match.group(2)) * 1000,
                                float(match.group(3)) * 1000
                            ]
                            position_found = True
                            break
                        except:
                            continue
            
            # Velocity line (VX, VY, VZ)
            elif 'VX=' in line or 'VX =' in line:
                patterns = [
                    r'VX\s*=\s*([-\d.E+-]+)\s+VY\s*=\s*([-\d.E+-]+)\s+VZ\s*=\s*([-\d.E+-]+)',
                    r'([-\d.E+-]+)\s+([-\d.E+-]+)\s+([-\d.E+-]+)',  # Just numbers
                ]
                for pattern in patterns:
                    match = re.search(pattern, line)
                    if match:
                        try:
                            data['velocity'] = [
                                float(match.group(1)) * 1000,  # Convert km/s to m/s
                                float(match.group(2)) * 1000,
                                float(match.group(3)) * 1000
                            ]
                            velocity_found = True
                            break
                        except:
                            continue
        
        # If we didn't find labeled vectors, try to parse the numeric format
        if not position_found or not velocity_found:
            # Sometimes the data is just in columns without labels
            for line in lines:
                # Skip empty lines and header lines
                if not line.strip() or '****' in line or '$$' in line:
                    continue
                
                # Try to parse as space-separated numbers
                parts = line.split()
                if len(parts) >= 6:
                    try:
                        # Assuming format: date time X Y Z VX VY VZ
                        # Skip date/time parts and get the vectors
                        numbers = []
                        for part in parts:
                            try:
                                num = float(part)
                                numbers.append(num)
                            except:
                                continue
                        
                        if len(numbers) >= 6:
                            data['position'] = [numbers[0] * 1000, numbers[1] * 1000, numbers[2] * 1000]
                            data['velocity'] = [numbers[3] * 1000, numbers[4] * 1000, numbers[5] * 1000]
                            position_found = True
                            velocity_found = True
                            break
                    except:
                        continue
    
    if 'position' in data and 'velocity' in data:
        return data
    else:
        # Debug: print first 2000 chars of response to see what we're getting
        print(f"Could not parse vectors for {object_name}")
        print("Response preview:")
        print(text[:2000])
        return None

def generate_cpp_code(asteroids_data):
    """Generate C++ code for adding asteroids to the simulation"""
    
    cpp_code = """    // --- Additional Asteroids from JPL Horizons ---
    // Data epoch: 2025-Aug-17 00:00:00.0000 TDB
    
"""
    
    for ast_id, name, color, data in asteroids_data:
        if data is None:
            continue
            
        # Clean up the name for C++ variable
        var_name = name.lower().replace(' ', '_').replace('-', '_')
        
        cpp_code += f"""    // Asteroid {ast_id}: {name}
    CelestialBody asteroid_{var_name}(
        {data['mass']:.3e},  // Mass in kg
        QVector3D({data['position'][0]:.7e}f, {data['position'][1]:.7e}f, {data['position'][2]:.7e}f),  // Position in meters
        QVector3D({data['velocity'][0]:.7e}f, {data['velocity'][1]:.7e}f, {data['velocity'][2]:.7e}f),  // Velocity in m/s
        {data['radius']:.1f},  // Radius in meters
        "{name}",
        QColor{color}
    );
    simulation.addBody(asteroid_{var_name});

"""
    
    return cpp_code

def main():
    print("Fetching asteroid data from JPL Horizons...")
    print(f"Epoch: {EPOCH}")
    print(f"Total asteroids to fetch: {len(ASTEROIDS)}")
    print("-" * 50)
    
    asteroids_data = []
    
    for i, (ast_id, name, color) in enumerate(ASTEROIDS):
        print(f"[{i+1}/{len(ASTEROIDS)}] Fetching {name} (ID: {ast_id})...")
        data = fetch_horizons_data(ast_id, name)
        asteroids_data.append((ast_id, name, color, data))
        if data:
            print(f"  ✓ Success: pos=({data['position'][0]/1e9:.2f}, {data['position'][1]/1e9:.2f}, {data['position'][2]/1e9:.2f}) Gm")
        else:
            print(f"  ✗ Failed - check if asteroid {ast_id} exists in Horizons")
    
    # Generate C++ code
    cpp_code = generate_cpp_code(asteroids_data)
    
    # Save to file
    with open("asteroids_code.cpp", "w") as f:
        f.write(cpp_code)
    
    print("-" * 50)
    print(f"Generated C++ code saved to 'asteroids_code.cpp'")
    print(f"Successfully fetched {sum(1 for _, _, _, d in asteroids_data if d is not None)} out of {len(ASTEROIDS)} asteroids")
    print("\nSimply copy and paste the generated code into your main.cpp file,")
    print("right after where you add the existing planets/dwarf planets!")

if __name__ == "__main__":
    main()