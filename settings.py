info = {
    "team":   "SimCar",
    "members":   "Vineet Baid, Will Drevno, Janaki Gunnam, Himanshu Jatia, Ahmed Osman, Mathieu Sidoti",
    "description":  
        """This is a simulation of Inventory Modeling of a Car Rental Company. 
           I hope you enjoy!""",
    "additional":
        """<h3>Full Version Capabilities:</h3>
        <ul>
            <li>Supports up to 50 locations</li>
            <li>Models inventories of up to 10 car types</li>
            <li>Simulates operations over 2 years</li>
        </ul>
        """
}

sigma = {
    "model": "34",    
    "runtime":{"name": "RUNTIME",
               "display": "Running Time (in Hours)",
               "default": 1000
              },    
	
	"data": [
        {   "display": "Distance between Cities",
            "filename": "Dist"
		},
		
		{   "display": "Arrivals for Each Car Type at Each City",
            "filename": "Arrival"
		},
		
		{   "display": "Inventory of Each Car Type at Each City",
            "filename": "Inventory"
		},
		
		{   "display": "Probability of a Car from one City going to Another",
            "filename": "Probability"
		},
		
		{   "display": "Beta Distribution for Use Time of Each Car Type at Each City",
            "filename": "Usetime"
		},
		
		{   "display": "Threshold Values for Each Car Type at Each City",
            "filename": "Threshold"
		},
		
		{   "display": "Monthly Seasonality Values",
            "filename": "Seasonality"
		},
	],
	
    "parameters": [
        {   "name": "N",
            "display": "Number of Locations",
            "default": 3
        },

        {   "name": "TYPE",
            "display": "Number of Car Types in Each Location",
            "default": 3
        },
		
		{   "name": "BATCH",
            "display": "Cars to be Transferred in Case Inventory Drops Below Threshold Value",
            "default": 5
        },
		
		{   "name": "GROWTH",
            "display": "Growth of Car Rentals per Year (in %)",
            "default": 5
        },
    ]
}

rdp_enable = True

graphs = [
    {   "graph_name": "Inventory of Different Car Types at Location 1 vs Time (in Hours)",
        "x-axis": "Time",
        "x-display": "Time (in Hours)",
        "y-axis": ["INV[0;0]", "INV[0;1]", "INV[0;2]"],
        "y-display": "Inventory of Different Car Types at Location 1",
        "lines": ["Ford Mustang", "Honda Civic", "Ford Explorer"],
        "rdp_epsilon": [5, 5, 5]
    },
	
	{   "graph_name": "Inventory of Different Car Types at Location 2 vs Time (in Hours)",
        "x-axis": "Time",
        "x-display": "Time (in Hours)",
        "y-axis": ["INV[1;0]", "INV[1;1]", "INV[1;2]"],
        "y-display": "Inventory of Different Car Types at Location 2",
        "lines": ["Ford Mustang", "Honda Civic", "Ford Explorer"],
        "rdp_epsilon": [5, 5, 5]
    },
	
	{   "graph_name": "Inventory of Different Car Types at Location 3 vs Time (in Hours)",
        "x-axis": "Time",
        "x-display": "Time (in Hours)",
        "y-axis": ["INV[2;0]", "INV[2;1]", "INV[2;2]"],
        "y-display": "Inventory of Different Car Types at Location 3",
        "lines": ["Ford Mustang", "Honda Civic", "Ford Explorer"],
        "rdp_epsilon": [5, 5, 5]
    },
	
	{   "graph_name": "Total Rejections vs Time (in Hours)",
        "x-axis": "Time",
        "x-display": "Time (in Hours)",
        "y-axis": ["REJECT"],
        "y-display": "Total Rejections",
        "lines": ["Rejections"],
        "rdp_epsilon": [2]
    },
]

server = {
    "hostname": "localhost",
    "port": 9000
}
