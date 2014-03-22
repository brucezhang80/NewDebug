---------------------------------------------------------------------------------------------------------
try(destroyDialog httpSock)catch()
rollout httpSock "httpSock" width:110 height:110 
(
	activeXControl port "Microsoft.XMLHTTP" setupEvents:false releaseOnClose:false
)
createDialog httpSock pos:[100,130];--destroyDialog httpSock;

struct rdt_updateObj
(
	itemID,
	version,
	url,
	path,
	ini
);

struct rdt_updateModule
(

	function geturlData inputURL =
	(
		if httpSock == undefined then
		(
			rollout httpSock "httpSock" width:0 height:0 (activeXControl port "Microsoft.XMLHTTP" setupEvents:false releaseOnClose:false);
			createDialog httpSock pos:[-100,-100];destroyDialog httpSock;
		);

		httpSock.port.open "GET" inputURL false;
		httpSock.port.setrequestheader "If-Modified-Since" "Sat, 1 Jan 1900 00:00:00 GMT";
		httpSock.port.send();
		return (httpSock.port.responseText);
	),
	function downloadObj sourceObj outFile =
	(
		local outfileStream = createFile outFile;
		if outfileStream == undefined then return undefined;
		format (rdt_updateModule.geturlData sourceObj.url) to:outfileStream;
		close outfileStream;

		return outFile;
	),

	function performUpdate oldObj newObj =
	(
		rdt_updateModule.downloadObj newObj oldObj.path;
		filein oldObj.path;
	),


	function getAttrib inputAttrib inputString =
	(
		local startAttrib = (findString inputString ("<"+inputAttrib as string+">"));
		local endAttrib = (findString inputString ("</"+inputAttrib as string+">"));

		if startAttrib == 0 or endAttrib == 0 then return undefined
		startAttrib += ("<"+inputAttrib as string+">").count;

		return (substring inputString startAttrib (endAttrib - startAttrib));
	),

	function getURLobj inputURL =
	(
		local collectedXML = (rdt_updateModule.geturlData inputURL);

		if collectedXML == "" then return undefined;

		local returnUpdateObj = rdt_updateObj();
		returnUpdateObj.itemID = execute (rdt_updateModule.getAttrib #itemID collectedXML)
		returnUpdateObj.version = execute (rdt_updateModule.getAttrib #version collectedXML)
		returnUpdateObj.url = (rdt_updateModule.getAttrib #url collectedXML)
		returnUpdateObj.ini = (rdt_updateModule.getAttrib #ini collectedXML)

		return returnUpdateObj;
	),

	function checkUpdate currentObj reEvaluate:false =
	(

		local updateInfo = (rdt_updateModule.getURLobj ("http://www.rodgreen.com/mxs/updater/rdt_update.php?itemID=" + currentObj.itemID as string));
		if updateInfo.version > currentObj.version then rdt_updateModule.performUpdate currentObj updateInfo;
		if reEvaluate then (filein currentObj.path);

		--if updateInfo.version == currentObj.version then ();
		--if updateInfo.version < currentObj.version then ();
	);

);

---------------------------------------------------------------------------------------------------------