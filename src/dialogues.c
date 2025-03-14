#include "dialogues.h"
#include "game.h"

DialogueEntry dialogues[] = {
    // Base location
    {"base", "base_intro", "You're at the base of the CN Tower. It's huge!", "Aww, sweetie, you're at the base of the CN Tower! It's so wonderful!"},
    {"base", "base_directions", "Entrance is North. Gift shop is East.", "The entrance is to the north, darling! And the gift shop is to the east, cutie!"},
    {"base", "base_alex", "You see a person who looks like they want to talk (West).", "You see someone who looks super friendly and they seem like they want to chat, sweetie! (West)"},
    {"base", "base_worker", "A worker is struggling with some boxes (South).", "Aww, a worker seems like they need help with some boxes, cutie! (South)"},
    {"base", "base_patrick", "You see a strange guy, he looks like a club member (West)", "You see a super cool person, they look like a member of a club, darling! (West)"},
    {"base", "base_what", "What do you want to do?", "So, sweetie, what's the vibe?"},
    {"base", "base_hints", "Hints: 'Go North', 'Go East', 'Go West', 'Go South', 'Look Around', 'Inventory', 'Help', 'Exit', 'Restart', 'Save', 'Load'.", "Hints, my darling! 'Go North', 'Go East', 'Go West', 'Go South', 'Look Around', 'Inventory', 'Help', 'Exit', 'Restart', 'Save', 'Load'. What do you wanna do, sweetie?"},

    // Alex Rivers
    {"alex_rivers", "alex_intro", "You go to the person. They say their name is Alex Rivers.", "You go to the person! They introduce themselves as Alex Rivers, what a sweetie!"},
    {"alex_rivers", "alex_greeting", "\"Hey! Nice day to visit the CN Tower, right?\"", "\"Hiya, darling! Isn't today just an amazing day to visit the CN Tower?\""},
    {"alex_rivers", "alex_watch1", "Alex checks their watch. \"It's 17:46. I have 5 minutes to record a video for my social media channel.\"", "Alex checks their watch and says: \"Oh my gosh, darling! It's 17:46! I have like 5 minutes to make an amazing video for my social media channel\""},
    {"alex_rivers", "alex_talking", "Alex talks a lot about the weather, the view, and their love for the CN Tower.", "Alex is just talking about how pretty the view is and how much they love the CN Tower! They are so passionate!"},
    {"alex_rivers", "alex_blah", "...blah, blah, blah! ({5-i} minutes)", "...blah, blah, blah! ({5-i} minutes)... they seem so passionate!"},
    {"alex_rivers", "alex_watch2", "...Alex looks at their watch.", "...Alex looks at their watch and seems startled, oh no, darling!"},
    {"alex_rivers", "alex_run", "\"Oh no! I lost track of time. Gotta run!\"", "\"Oh no, sweetie! I have to go now!\""},
    {"alex_rivers", "alex_time_wasted", "You wasted a lot of time.", "You spent a lot of time talking, darling, hehe."},
    {"alex_rivers", "alex_continue", "You continue your tour.", "You continue your wonderful adventure, cutie!"},
    {"alex_rivers", "alex_no_ticket", "Also, you don't have much time, so you didn't buy a ticket.", "Also, you don't have much time, so you didn't get a ticket yet, darling."},
    {"alex_rivers", "alex_again", "It's Alex Rivers again. Still talking about the CN Tower.", "Oh, it's Alex Rivers again! They still seem obsessed with this tower, oh my!"},
    {"alex_rivers", "alex_you_again", "Alex: \"Oh, it's you! Enjoying the tower? It's great, right?\"", "Alex: \"Oh hey again, sweetie! Are you having fun exploring the tower? It's beautiful, isn't it?\""},
    {"alex_rivers", "alex_what", "What do you want to do?", "What would you like to do, cutie?"},
    {"alex_rivers", "alex_support1", "This tower is truly a marvel of engineering!", "This tower is such a marvel of engineering, wow!"},
    {"alex_rivers", "alex_support2", "The view from up here is absolutely breathtaking!", "The view from up here is absolutely stunning, darling!"},
    {"alex_rivers", "alex_support3", "You're doing a great job promoting this place, Alex!", "You're doing such a great job at promoting this place, Alex! You are amazing!"},
    {"alex_rivers", "alex_support4", "I've never seen anything like this before!", "I have never seen something like this before, what a wonderful view!"},
    {"alex_rivers", "alex_support5", "This is the best day of my life!", "This is the best day ever, sweetie!"},
    {"alex_rivers", "alex_thanks", "Alex: \"Wow, you think so? That's awesome! Here, take $40. Also i'll give you a ticket and a mask\"", "Alex: \"Aww, you think so, darling? That's amazing! Take $40 and here, a ticket and a mask, sweetie!\""},
    {"alex_rivers", "alex_thanks2", "Alex: \"Thanks! Every little bit helps.\"", "Alex: \"Thank you, sweetie! It means a lot.\""},
	{"alex_rivers", "alex_hints", "Hints: 'Compliment Alex', 'Ignore', 'Exit', 'Restart'.", "Hints, my darling! 'Compliment Alex', 'Ignore', 'Exit', 'Restart'. What are you gonna choose?"},

    // Patrick
    {"Patrick", "patrick_intro", "You go to the strange guy. He says his name is Patrick.", "You go to the friendly person! They introduce themselves as Patrick, what a cutie!"},
    {"Patrick", "patrick_greeting", "\"Hey! You look like you've got energy. Join our quadrobics club?\"", "\"Hi, darling! You seem so energetic! Do you wanna join our quadrobics group, sweetie?\""},
    {"Patrick", "patrick_moves", "Patrick shows some quadrobics moves.", "Patrick shows you some quadrobics moves, very cool, darling!"},
    {"Patrick", "patrick_what", "What do you want to do?", "What would you like to do, darling?"},
    {"Patrick", "patrick_hints", "Hints: 'Join', 'Decline', 'Back', 'Exit', 'Restart'.", "Hints, sweetie! 'Join', 'Decline', 'Back', 'Exit', 'Restart'. Your move!"},

    // Entrance
    {"entrance", "entrance_line", "You're at the entrance. There's a long line for tickets.", "You are at the entrance, sweetie, so many people are waiting for tickets!"},
    {"entrance", "entrance_ticket", "You have a ticket! Go to security check (North).", "You have a ticket, amazing! Go straight to security check to the north, cutie!"},
    {"entrance", "entrance_no_ticket", "You need a ticket. Buy one at the ticket booth (West).", "You need a ticket, sweetie! Go buy it at the ticket booth to the west!"},
    {"entrance", "entrance_what", "What do you want to do?", "So, sweetie? What would you like to do?"},
    {"entrance", "entrance_hints", "Hints: 'Go North' (with ticket), 'Go West', 'Back', 'Exit', 'Restart'.", "Hints, cutie! 'Go North', 'Go West', 'Back', 'Exit', 'Restart'. Your choice!"},

    // ... (Add all other locations and their dialogue entries here) ...
	//Ticket Booth
	{"ticket_booth", "ticket_price", "You're at the ticket booth. Tickets are $40.", "You are at the ticket booth, sweetie! Tickets are $40."},
    {"ticket_booth", "ticket_what", "What do you want to do?", "What would you like to do, darling?"},
    {"ticket_booth", "ticket_hints", "Hints: 'Buy Ticket', 'Back', 'Exit', 'Restart'.", "Hints, sweetie! 'Buy Ticket', 'Back', 'Exit', 'Restart'. What are we buying?"},
	// Security
	{"security", "security_check", "You're at the security check. They're checking bags and tickets.", "You are at the security check! They are checking all bags and tickets, sweetie!"},
	{"security", "security_pass", "The guard checks your ticket and lets you through to the elevator (North).", "The guard checks your ticket and allows you to pass to the elevator to the north, cutie!"},
	{"security", "security_no_ticket", "You need a ticket to go through.", "You need a ticket to go through, darling!"},
	{"security", "security_what", "What do you want to do?", "What's your next move, darling?"},
	{"security", "security_hints", "Hints: 'Go North' (with ticket), 'Back', 'Exit', 'Restart'.", "Hints, sweetie! 'Go North' (with ticket), 'Back', 'Exit', 'Restart'. So?"},
	// Elevator
	{"elevator", "elevator_close", "You're in the elevator. The doors close.", "You are in the elevator and the doors are closing, sweetie!"},
	{"elevator", "elevator_up", "Going up fast...", "Going upppp, cutie!"},
	{"elevator", "elevator_pop", "Your ears pop.", "Your ears are popping, darling!"},
	{"elevator", "elevator_ding", "Ding! LookOut level.", "Ding! You are at the LookOut level, sweetie!"},
	// LookOut
	{"lookout", "lookout_view", "You're on the LookOut level! Great view of Toronto.", "You made it to the LookOut level, darling! The view of Toronto is astonishing!"},
	{"lookout", "lookout_see", "You see the city, the lake, and Niagara Falls far away.", "You can see the city, the lake, and if you look far enough, Niagara Falls, cutie!"},
	{"lookout", "lookout_directions", "Stairs to Glass Floor (Down). Info booth (East).", "Stairs to the Glass Floor are to the south! The info booth is to the east, sweetie!"},
	{"lookout", "lookout_what", "What do you want to do?", "What do you wanna explore, darling?"},
	{"lookout", "lookout_hints", "Hints: 'Go Down', 'Go East', 'Look Around', 'Exit', 'Restart'.", "Hints, cutie! 'Go Down', 'Go East', 'Look Around', 'Exit', 'Restart'. What do you wanna do next?"},
	//Glass Floor
	{"glass_floor", "glass_scary", "You're on the Glass Floor! It's scary to look down.", "You are on the Glass Floor, sweetie! Looking down is so scary!"},
	{"glass_floor", "glass_see", "You see the ground 342 meters below.", "You can see the ground 342 meters below you, wow, darling!"},
	{"glass_floor", "glass_directions", "Stairs up to LookOut level. EdgeWalk sign (West).", "The stairs back up to the LookOut level are to the north. The EdgeWalk sign is to the west, cutie."},
	{"glass_floor", "glass_what", "What do you want to do?", "What are we doing next, darling?"},
	{"glass_floor", "glass_hints", "Hints: 'Go Up', 'Go West', 'Look Down', 'Exit', 'Restart'.", "Hints, sweetie! 'Go Up', 'Go West', 'Look Down', 'Exit', 'Restart'. Your choice?"},
	//Edge Walk
	{"edgewalk_registration", "edgewalk_desk", "You're at the EdgeWalk desk.", "You have arrived at the EdgeWalk desk, my darling!"},
	{"edgewalk_registration", "edgewalk_ticket", "You have a ticket! The guide is preparing the gear (North).", "You have a ticket, amazing! The guide is preparing the gear to the north, sweetie!"},
	{"edgewalk_registration", "edgewalk_no_ticket", "You need a ticket for EdgeWalk. It's $195. Buy one here.", "You need a ticket for the EdgeWalk, it's $195, darling! Buy one here!"},
	{"edgewalk_registration", "edgewalk_what", "What do you want to do?", "What are we choosing next, cutie?"},
	{"edgewalk_registration", "edgewalk_hints", "Hints: 'Buy Ticket', 'Go North' (with ticket), 'Back', 'Exit', 'Restart'.", "Hints, sweetie! 'Buy Ticket', 'Go North' (with ticket), 'Back', 'Exit', 'Restart'. So what do you wanna do?"},
	// Edge Walk Preparation.
	{"edgewalk_preparation", "edgewalk_prep", "You're in the EdgeWalk prep area. The guide helps you put on a harness.", "You are in the EdgeWalk preparation area, my darling! The guide is helping you put on the harness!"},
	{"edgewalk_preparation", "edgewalk_nervous", "You're excited and nervous.", "You are excited and nervous, this is really happening, cutie!"},
	{"edgewalk_preparation", "edgewalk_check", "The guide checks your harness. Thumbs up!", "The guide checks your harness! Thumbs up, let's go, sweetie!"},
	// Edge Walk
	{"edgewalk", "edgewalk_outside", "You're outside on the EdgeWalk! Wind is blowing. You're walking around the CN Tower!", "You are outside on the EdgeWalk, the wind is blowing! You are walking around the CN Tower, darling!"},
	{"edgewalk", "edgewalk_exciting", "It's the most exciting thing ever!", "This is the most exciting thing ever, cutie!"},
	{"edgewalk", "edgewalk_win", "Congrats! You did the EdgeWalk! (Win)", "Congratulations, darling! You did the EdgeWalk! (Win), you did it!"},
	// Gift Shop
	{"gift_shop", "gift_souvenirs", "You're in the gift shop. They have souvenirs, postcards, and CN Tower stuff.", "You are in the gift shop, sweetie! They sell souvenirs, postcards, and cool CN Tower things!"},
	{"gift_shop", "gift_mask", "You see a disguise kit for $20.", "You see a disguise kit here that is $20, my darling!"},
	{"gift_shop", "gift_what", "What do you want to do?", "What are you gonna choose, sweetie?"},
	{"gift_shop", "gift_hints", "Hints: 'Buy Postcards', 'Buy Souvenir', 'Buy Mask' (with enough money), 'Back', 'Exit', 'Restart'.", "Hints, darling! 'Buy Postcards', 'Buy Souvenir', 'Buy Mask' (with enough money), 'Back', 'Exit', 'Restart'. What are we buying?"},
	// Information Booth
	{"information_booth", "info_brochures", "You're at the info booth. Brochures about the CN Tower are here.", "You are at the info booth, my sweetie. There are brochures about the CN Tower here!"},
	{"information_booth", "info_staff", "A staff member is answering questions.", "A staff member is here, darling, ready to answer all the questions!"},
	{"information_booth", "info_what", "What do you want to do?", "What do you want to do, cutie?"},
	{"information_booth", "info_hints", "Hints: 'Ask About History', 'Ask About Building', 'Back', 'Exit', 'Restart'.", "Hints, darling! 'Ask About History', 'Ask About Building', 'Back', 'Exit', 'Restart'. What do you wanna know?"},
	// Worker
	{"worker", "worker_tired", "You go to the worker. He looks tired.", "You walk to the worker, they seem very tired, sweetie."},
	{"worker", "worker_help", "\"Hey, can you help me? I need to move these boxes to the storage room.\"", "\"Hi, cutie, can you help me? I need to get these boxes to the storage room.\""},
	{"worker", "worker_start", "You start helping.", "You are now helping out, amazing!"},
	{"worker", "worker_carry", "You carry box {i} to the storage room...", "You carry box {i} to the storage room..."},
	{"worker", "worker_fifth", "You pick up the 5th box. It's open a bit.", "You pick up the 5th box, it seems open a bit, darling!"},
	{"worker", "worker_what", "What do you want to do?", "What are you going to do, darling?"},
	{"worker", "worker_hints", "Hints: 'Look Inside', 'Continue', 'Exit', 'Restart'.", "Hints, sweetie! 'Look Inside', 'Continue', 'Exit', 'Restart'. So?"},
	// Open Box
	{"open_box", "box_contents", "You look inside. You see money, a book, and a mask.", "You look inside, cutie! You can see money, a book, and a mask."},
	{"open_box", "box_what", "What do you want to do?", "What's your choice, sweetie?"},
	{"open_box", "box_hints", "Hints: 'Take Nothing', 'Take Money', 'Take Book', 'Take Mask', 'Exit', 'Restart'.", "Hints, darling! 'Take Nothing', 'Take Money', 'Take Book', 'Take Mask', 'Exit', 'Restart'. What will you do?"},
    // Caught Stealing
	{"caught_stealing", "caught_seen", "The worker sees you!", "Oh no, darling! The worker sees you!"},
	{"caught_stealing", "caught_worker", "Worker: \"Hey! What are you doing?!\"", "Worker: \"Hey, sweetie! What do you think you are doing?!\""},
	{"caught_stealing", "caught_security", "He calls security. You're taken to the police.", "He calls security, you are being taken to the police, sweetie!"},
	{"caught_stealing", "caught_what", "What do you want to do?", "What now, darling?"},
	{"caught_stealing", "caught_hints", "Hints: 'Tell Truth', 'Bribe', 'Lie', 'Exit', 'Restart'.", "Hints, darling! 'Tell Truth', 'Bribe', 'Lie', 'Exit', 'Restart'. Your move!"},
	// Police Station
	{"police_station", "police_questions", "You're at the police station. The officer is asking you questions.", "You are now at the police station, darling, the officer is asking you all the questions!"},
	{"police_station", "police_what", "What do you want to do?", "What now, sweetie?"},
	{"police_station", "police_hints", "Hints: 'Tell Truth', 'Bribe', 'Lie', 'Exit', 'Restart'.", "Hints, cutie! 'Tell Truth', 'Bribe', 'Lie', 'Exit', 'Restart'. What do you choose?"},
	// Storage Room
	{"storage_room", "storage_thanks", "You're in the storage room with the worker.", "You are in the storage room with the worker, sweetie!"},
	{"storage_room", "storage_reward", "Worker: \"Thanks a lot! Here's $20.\"", "Worker: \"Thank you, darling! Here is $20!\""},
	{"storage_room", "storage_money", "You got $20.", "You get $20, amazing!"},
	{"storage_room", "storage_what", "What do you want to do?", "So what next, darling?"},
	{"storage_room", "storage_hints", "Hints: 'Back', 'Exit', 'Restart'.", "Hints, sweetie! 'Back', 'Exit', 'Restart'. What are we choosing?"},
    // Just a Chill Guy
	{"just_a_chill_guy", "chill_laughing", "You see Just a Chill Guy. He's laughing, looking at a corner.", "You can see Just a Chill Guy, darling! They are laughing while looking at a corner."},
	{"just_a_chill_guy", "chill_ready", "You have a mask and a Bible. You're ready to scare Alex Rivers!", "You have the mask and the Bible, you are ready to scare Alex Rivers, amazing!"},
	{"just_a_chill_guy", "chill_quadrobists_gone", "Just a Chill Guy: \"Now that the quadrobists ran away, you can scare Alex using the mask.\"", "Just a Chill Guy says: \"Now that the quadrobists are gone, you can scare Alex with the mask, sweetie!\""},
	{"just_a_chill_guy", "chill_scared_quadrobists", "Just a Chill Guy: \"You scared the quadrobists good, now you can scare Alex.\"", "Just a Chill Guy says: \"You have scared the quadrobists! Now go scare Alex, friend!\""},
	{"just_a_chill_guy", "chill_what", "What do you want to do?", "What do you wanna do, darling?"},
	{"just_a_chill_guy", "chill_hints1", "Hints: 'Use Mask', 'Go Forward', 'Ask About Corner', 'Back', 'Exit', 'Restart'.", "Hints, sweetie! 'Use Mask', 'Go Forward', 'Ask About Corner', 'Back', 'Exit', 'Restart'. What are you deciding?"},
    {"just_a_chill_guy", "chill_hints2", "Hints: 'Use Mask',  'Go Forward', 'Ask About Corner', 'Back', 'Exit', 'Restart'.","Hints, sweetie! 'Use Mask',  'Go Forward', 'Ask About Corner', 'Back', 'Exit', 'Restart'. What are you deciding?"},
    {"just_a_chill_guy", "chill_hints3", "Hints: 'Use Mask', 'Use Bible', 'Go Forward', 'Ask About Corner', 'Back', 'Exit', 'Restart'.","Hints, sweetie! 'Use Mask', 'Use Bible', 'Go Forward', 'Ask About Corner', 'Back', 'Exit', 'Restart'."},

    // Corner
	{"corner", "corner_peek", "You use your mask and peek around the corner. You see quadrobists practicing.", "You use the mask and look around the corner, you see the quadrobists practicing, cutie."},
	{"corner", "corner_back", "They don't see you. You go back to Just a Chill Guy.", "They don't see you! You go back to Just a Chill Guy, darling!"},
	{"corner", "corner_seen", "You go to the corner, and quadrobists see you!", "You go to the corner and the quadrobists see you, darling!"},
	{"corner", "corner_join", "They make you join their quadrobics training.", "They make you join their quadrobics training now, sweetie!"},
	{"corner", "corner_quadrobist", "Now you're a quadrobist. You must scare Alex Rivers.", "You are now a quadrobist! You have to go scare Alex Rivers, darling!"},
	// Quadrobics Base
	{"quadrobics_base", "quadrobics_move", "You move like a quadrobist. Alex is West.", "You are now moving like a quadrobist, Alex is to the west, darling!"},
	{"quadrobics_base", "quadrobics_hints", "Hints: 'Go West', 'Exit', 'Restart'.", "Hints, cutie! 'Go West', 'Exit', 'Restart'. So what's next?"},
	// Scare Alex
	{"scare_alex", "scare_success", "You successfully scared Alex Rivers using the mask and the Bible!", "You have successfully scared Alex Rivers using the mask and the Bible, amazing!"},
	{"scare_alex", "scare_drop", "Alex runs away, dropping their phone. You see your chance!", "Alex runs away, dropping their phone! You see your chance to get their phone, sweetie!"},
	{"scare_alex", "scare_hints", "Hints: 'Take Phone', 'Leave Phone', 'Exit', 'Restart'", "Hints, darling! 'Take Phone', 'Leave Phone', 'Exit', 'Restart'. What are you choosing?"},
    // Phone Found
	{"phone_found", "phone_run", "You take the phone and run to the edge of the roof. There are no obstacles in front of you.", "You take the phone and run to the edge of the roof, you can see no obstacles in front of you, sweetie."},
	{"phone_found", "phone_what", "What do you want to do?", "What are you going to do, cutie?"},
	{"phone_found", "phone_hints", "Hints: 'Jump', 'Go Back' to the Glass Floor, 'Exit', 'Restart'", "Hints, darling! 'Jump', 'Go Back' to the Glass Floor, 'Exit', 'Restart'. What's the move?"},
	// Roof
	{"roof", "roof_jump", "You jumped from the roof. The last thing you see is blue sky", "You jumped from the roof, the last thing you see is the beautiful blue sky, sweetie."},
    // Information Booth Dialogues
    {"information_booth", "history1", "CN Tower: Built in 1976, once the tallest structure (553.3 m).", "CN Tower: Built in 1976, once the tallest structure (553.3 m)."},
	{"information_booth", "history2", "Built by Canadian National Railway. Now a tourist spot.", "Built by Canadian National Railway. Now a tourist spot."},
    {"information_booth", "history3", "It can handle earthquakes and winds. Has a core with elevators and stairs.", "It can handle earthquakes and winds. Has a core with elevators and stairs."},
    {"information_booth", "building1", "It took 40 months to build with work done 24/7.", "It took 40 months to build with work done 24/7."},
    {"information_booth", "building2", "A big helicopter lifted the antenna. Built with a 'slipform' method.", "A big helicopter lifted the antenna. Built with a 'slipform' method."},
    {"information_booth", "building3", "Foundation is 15 m deep, with 7,000 cubic meters of concrete.", "Foundation is 15 m deep, with 7,000 cubic meters of concrete."},


    {NULL, NULL, NULL, NULL} // Terminator entry (IMPORTANT!)
};

const char *get_dialogue(const char *location, const char *key, bool sweet_mode) {
    for (int i = 0; dialogues[i].key != NULL; i++) {
        if (strcmp(dialogues[i].key, location) == 0 && strcmp(dialogues[i].text, key) == 0) {
            return sweet_mode ? dialogues[i].sweet_text : dialogues[i].text;
        }
    }
    return "Dialogue not found"; // Should not happen in normal gameplay
}