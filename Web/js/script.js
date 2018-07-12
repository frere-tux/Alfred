function changeState(pin,elem){
	var newState = ($(elem).hasClass('on')?0:1);
	$.ajax({
			type: "POST",
			 url: "./action.php?action=changeState",
			 data:{pin:pin,state:newState},
			success: function(r){
				var result = eval(r);
				if(result.state == 1){
					$(elem).removeClass('on');
					$(elem).removeClass('off');
					$(elem).addClass((newState==1?'on':'off'));
				}else{
					alert('Erreur : '+result.error);
				}
		 }});
}

function demo(){
	$.ajax({
			type: "POST",
			 url: "./action.php?action=demo",
			success: function(r){
		 }});
}

function playSound(){
	$.ajax({
			type: "POST",
			url: "./action.php?action=playSound",
			success: function(r){
		 }});
}

function turnOn(){
	$.ajax({
			type: "POST",
			url: "./action.php?action=turnOn",
			success: function(r){
		 }});
}

function turnOff(){
	$.ajax({
			type: "POST",
			url: "./action.php?action=turnOff",
			success: function(r){
		 }});
}

function sendSimpleRequest(groupID,elementID,state,group){
	$.ajax({
                type: "POST",
                url: "./SendSimpleRequest.php",
                data:{groupID:groupID,elementID:elementID,state:state,group:group},

                success : function(){
                },

                error : function(){
                    alert('error');
                },

                complete : function(){
                }
            });
}

function getRequests(requestListElement)
{
    var requestsArray = document.getElementById(requestListElement);
    var requests = new Array();

    for (var i = 0 ; i < requestsArray.childNodes.length ; i++)
    {
        var childRequest = requestsArray.childNodes[i];
        if (childRequest.className == "requestForm")
        {
            var roomObject = 0;
            var state = false;

            for (var j = 0 ; j < childRequest.childNodes.length ; j++)
            {
                var childInput = childRequest.childNodes[j];

                if (childInput.className == "requestObject")
                {
                    roomObject = childInput.value;
                }
                else if (childInput.className == "requestState")
                {
                    state = childInput.checked;
                }
            }

            if (roomObject == 0)
            {
                return;
            }

            var split = roomObject.split(":");
            var room = split[0];
            var object = split[1];

            var request = {room:room, object:object, state:state};
            requests.push(request);
        }
    }

    return requests;
}

function sendTask(){
    var startDate = document.getElementById('taskStartDate').value;
    var startTime = document.getElementById('taskStartTime').value;
    var endDate = document.getElementById('taskEndDate').value;
    var endTime = document.getElementById('taskEndTime').value;

    var periodicity = document.getElementById('taskPeriodicity').value;
    var periodicityUnit = document.getElementById('taskPeriodicityUnit').value;
    switch (periodicityUnit)
    {
        case "days":
            periodicity *= 60 * 60 * 24;
            break;
        case "hours":
            periodicity *= 60 * 60;
            break;
        case "minutes":
            periodicity *= 60;
            break;
        case "seconds":
        default:
    }

    var duration = document.getElementById('taskDuration').value;
    var durationUnit = document.getElementById('taskDurationUnit').value;
    switch (durationUnit)
    {
        case "days":
            duration *= 60 * 60 * 24;
            break;
        case "hours":
            duration *= 60 * 60;
            break;
        case "minutes":
            duration *= 60;
            break;
        case "seconds":
        default:
    }

    var startRequests = getRequests("startRequests");
    var endRequests = getRequests("endRequests");

    var startRequestsStr = JSON.stringify(startRequests);
    var endRequestsStr = JSON.stringify(endRequests);

	$.ajax({
                type: "POST",
                url: "./SendTask.php",
                data:{startDate:startDate,startTime:startTime,endDate:endDate,endTime:endTime,periodicity:periodicity,periodicityUnit:periodicityUnit,duration:duration,durationUnit:durationUnit,startRequestsStr:startRequestsStr,endRequestsStr:endRequestsStr},

                success : function(){
                },

                error : function(){
                    alert('error');
                },

                complete : function(){
                    alert('Nouvelle tache enregistrée');
                }
            });
}

function saveText(){
	//document.getElementById('inputToSave').preventDefault();
	var inputText = document.getElementById('inputToSave');
	$.ajax({
			type: "POST",
			url: "./action.php?action=saveText",
			data: 'text=' + inputText.value,

			success : function(){
				inputText.value = '';
            },

            error : function(){
				alert('Text not sent');
            },

            complete : function(){
            }
		});
}

function addElement(elementId, containerId)
{
    var elementToCopy = document.getElementById(elementId);
    var elementClone = elementToCopy.cloneNode(true);
    elementClone.id = "";
    elementClone.className = "requestForm"
    elementClone.style.display = "block";
    document.getElementById(containerId).insertBefore(elementClone, elementToCopy.nextSibling);
}

function removeElement(element)
{
    element.parentNode.remove();
}



