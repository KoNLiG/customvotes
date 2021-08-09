#if defined _customvotes_included
 #endinput
#endif
#define _customvotes_included

#define SERVER_OPERATOR_ID 0
#define DEFAULT_PASS_PERCENTAGE 50.0

/**
 * Client's custom vote decisions.
 */
enum
{
	VOTE_DECISION_NONE = -1,
	VOTE_DECISION_YES,
	VOTE_DECISION_NO
}

/**
 * Vote issue. These are mapped to translation strings and pass strings by VoteStart and VotePass
 * https://github.com/perilouswithadollarsign/cstrike15_src/blob/f82112a2388b841d72cb62ca48ab1846dfcc11c8/game/shared/shareddefs.h#L185-L206
 */
enum
{
	VOTE_ISSUE_UNDEFINED = 1, 
	VOTE_ISSUE_KICK, 
	VOTE_ISSUE_CHANGELEVEL, 
	VOTE_ISSUE_NEXTLEVEL, 
	VOTE_ISSUE_SWAPTEAMS, 
	VOTE_ISSUE_SCRAMBLE, 
	VOTE_ISSUE_RESTARTGAME, 
	VOTE_ISSUE_SURRENDER, 
	VOTE_ISSUE_REMATCH, 
	VOTE_ISSUE_CONTINUE, 
	VOTE_ISSUE_PAUSEMATCH, 
	VOTE_ISSUE_UNPAUSEMATCH, 
	VOTE_ISSUE_LOADBACKUP, 
	VOTE_ISSUE_ENDWARMUP, 
	VOTE_ISSUE_STARTTIMEOUT, 
	VOTE_ISSUE_ENDTIMEOUT, 
	VOTE_ISSUE_READYFORMATCH, 
	VOTE_ISSUE_NOTREADYFORMATCH, 
	
	// Represents the highest possible index of the enum
	VOTE_ISSUE_MAX
}

/**
 * Reasons a vote was failed.
 * https://github.com/perilouswithadollarsign/cstrike15_src/blob/f82112a2388b841d72cb62ca48ab1846dfcc11c8/game/shared/shareddefs.h#L142-L179
 */
enum
{
	VOTE_FAILED_GENERIC = 0, 
	VOTE_FAILED_TRANSITIONING_PLAYERS, 
	VOTE_FAILED_RATE_EXCEEDED,  // "time" is used
	VOTE_FAILED_YES_MUST_EXCEED_NO, 
	VOTE_FAILED_QUORUM_FAILURE, 
	VOTE_FAILED_ISSUE_DISABLED, 
	VOTE_FAILED_MAP_NOT_FOUND, 
	VOTE_FAILED_MAP_NAME_REQUIRED, 
	VOTE_FAILED_FAILED_RECENTLY,  // "time" is used
	VOTE_FAILED_FAILED_RECENT_KICK,  // "time" is used
	VOTE_FAILED_FAILED_RECENT_CHANGEMAP,  // "time" is used
	VOTE_FAILED_FAILED_RECENT_SWAPTEAMS,  // "time" is used
	VOTE_FAILED_FAILED_RECENT_SCRAMBLETEAMS,  // "time" is used
	VOTE_FAILED_FAILED_RECENT_RESTART,  // "time" is used
	VOTE_FAILED_TEAM_CANT_CALL, 
	VOTE_FAILED_WAITINGFORPLAYERS, 
	VOTE_FAILED_PLAYERNOTFOUND,  // Deprecated, same as generic failure
	VOTE_FAILED_CANNOT_KICK_ADMIN, 
	VOTE_FAILED_SCRAMBLE_IN_PROGRESS, 
	VOTE_FAILED_SWAP_IN_PROGRESS, 
	VOTE_FAILED_SPECTATOR, 
	VOTE_FAILED_DISABLED, 
	VOTE_FAILED_NEXTLEVEL_SET, 
	VOTE_FAILED_REMATCH, 
	VOTE_FAILED_TOO_EARLY_SURRENDER, 
	VOTE_FAILED_CONTINUE, 
	VOTE_FAILED_MATCH_PAUSED, 
	VOTE_FAILED_MATCH_NOT_PAUSED, 
	VOTE_FAILED_NOT_IN_WARMUP, 
	VOTE_FAILED_NOT_10_PLAYERS, 
	VOTE_FAILED_TIMEOUT_ACTIVE, 
	VOTE_FAILED_TIMEOUT_INACTIVE,  // Deprecated, same as generic failure
	VOTE_FAILED_TIMEOUT_EXHAUSTED, 
	VOTE_FAILED_CANT_ROUND_END, 
	
	// Represents the highest possible index of the enum
	VOTE_FAILED_MAX
}

enum struct CustomVoteSetup
{
	// Team id to broadcast the custom vote.
	//
	// CS_TEAM_NONE (0)   	   To everyone
	// CS_TEAM_SPECTATOR (1)   To spectators only
	// CS_TEAM_T (2) 		   To terrorists only
	// CS_TEAM_CT (3)   	   To counter-terrorists only
	int team;
	
	// Custom vote initiator client index (who executed the vote), or SERVER_OPERATOR_ID for console. 
	int initiator;
	
	// Custom vote issue id to use, see the enum above.
	int issue_id;
	
	// The required percenage of players who voted positively for the custom vote to pass.
	// Leaving the variable value as 0.0 will automatically set it to 'DEFAULT_PASS_PERCENTAGE'.
	float pass_percentage;
	
	// Disposition string to display on the custom vote panel once the vote has executed. (HTML based text)
	char dispstr[512];
	
	// Disposition string to display on the custom vote panel once the vote has passed. (HTML based text)
	char disppass[512];
	
	// Resets back everything to default values.
	void Reset()
	{
		this.team = 0;
		this.initiator = 0;
		this.issue_id = 0;
		this.pass_percentage = 0.0;
		this.dispstr[0] = '\0';
		this.disppass[0] = '\0';
	}
}

/**
 *	Called when a custom vote result is available.
 *	
 *	@param results	    		Array that contains the decisions of the voters.
 */
typedef CustomVote_Callback = function void (int results[MAXPLAYERS + 1]);

/**
 *	Executes a custom CS:GO panel vote.
 *	
 *	@param setup			    A fully configurated custom	vote setup data struct.
 *  @param timeout_duration		Time in seconds for the custom vote to display until timeout.
 *  @param passed_callback      Callback function to call once the vote has passed.
 *  @param failed_callback      Callback function to call once the vote has failed.
 *
 *  @error 						Another custom vote is currently in progress.
 */
native void CustomVotes_Execute(any[] setup, int timeout_duration, CustomVote_Callback passed_callback = INVALID_FUNCTION, CustomVote_Callback failed_callback = INVALID_FUNCTION);

/**
 * Returns whether a custom vote is in progress.
 *
 * @return              		True if a custom vote is in progress, false otherwise.
 */
native bool CustomVotes_IsVoteInProgress();

/**
 * Returns the configurated custom vote setup data struct of the current vote in progress.
 * 
 * @param buffer				Buffer to store the setup data.
 * @error						No custom vote is currently in progress.
 */
native void CustomVotes_GetSetup(any[] buffer);

/**
 * Cancels the current vote in progress.
 * 
 * @param force         		If set to true, this forces the custom vote to be cancelled, even if it's not being executed from the called plugin.
 * @error						No custom vote is currently in progress.
 */
native void CustomVotes_Cancel(bool force = false);

/**
 * Called when a custom vote is receiving from a client.
 * 
 * @param client             	Client index.
 * @param voteDecision		  	Client's vote decision. See the enum above.
 * @return             		  	An Action value. Returning Plugin_Handled bypasses the game function call.
 *                      	  	Returning Plugin_Stop bypasses the post hook as well as the game function.
 */
forward Action CustomVotes_OnVoteReceive(int client, int &voteDecision);

#if !defined REQUIRE_PLUGIN
public void __pl_customvotes_SetNTVOptional()
{
	MarkNativeAsOptional("CustomVotes_Execute");
	MarkNativeAsOptional("CustomVotes_IsVoteInProgress");
	MarkNativeAsOptional("CustomVotes_GetSetup");
	MarkNativeAsOptional("CustomVotes_Cancel");
}
#endif

public SharedPlugin __pl_customvotes = 
{
	name = "customvotes", 
	file = "customvotes.smx", 
#if defined REQUIRE_PLUGIN
    required = 1, 
#else
    required = 0, 
#endif
}; 