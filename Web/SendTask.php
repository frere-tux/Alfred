#!/usr/bin/env php
<?php

//namespace tutorial\php;

error_reporting(E_ALL);

require_once('common.php');
require_once '/home/pi/Projects/External/thrift-0.9.3/lib/php/lib/Thrift/ClassLoader/ThriftClassLoader.php';

use Thrift\ClassLoader\ThriftClassLoader;

$GEN_DIR = '/var/www/html/gen-php';

$loader = new ThriftClassLoader();
$loader->registerNamespace('Thrift', '/home/pi/Projects/External/thrift-0.9.3/lib/php/lib');
$loader->registerDefinition('AlCom', $GEN_DIR);
$loader->register();

/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

use Thrift\Protocol\TBinaryProtocol;
use Thrift\Transport\TSocket;
use Thrift\Transport\THttpClient;
use Thrift\Transport\TBufferedTransport;
use Thrift\Exception\TException;

try {

  function stringToTimestamp($date, $time)
  {
    $dateTime = DateTime::createFromFormat('Y-m-d', $date);
    list($hours, $minutes) = explode(':', $time);

    $dateTime->setTime($hours, $minutes);

    return $dateTime->GetTimestamp();
  }

  function getComRequests($requestsArray)
  {
    $comRequests = array();
    foreach ($requestsArray as $entry)
    {
        $requestStart = new \AlCom\SimpleRequest();
        $requestStart->groupID = $entry->{'room'};

        if ($entry->{'object'} == -1)
        {
            $requestStart->elementID = 0;
            $requestStart->group = true;
        }
        else
        {
            $requestStart->elementID = $entry->{'object'};
            $requestStart->group = false;
        }

        $requestStart->state = $entry->{'state'};
        $comRequests[] = $requestStart;
    }
    return $comRequests;
  }




  $task = new \AlCom\ComTask();
  $task->startTime = stringToTimestamp($_['startDate'], $_['startTime']);
  $task->endTime = stringToTimestamp($_['endDate'], $_['endTime']);
  $task->duration = $_['duration'];
  $task->periodicity = $_['periodicity'];

  $task->startRequests = getComRequests(json_decode($_['startRequestsStr']));

  $task->endRequests = getComRequests(json_decode($_['endRequestsStr']));


  if (array_search('--http', $argv)) {
    $socket = new THttpClient('localhost', 8080, '/php/PhpServer.php');
  } else {
    $socket = new TSocket('localhost', 9090);
  }
  $transport = new TBufferedTransport($socket, 1024, 1024);
  $protocol = new TBinaryProtocol($transport);
  $client = new \AlCom\AlfredCommunicationClient($protocol);

  $transport->open();

  $client->sendTask($task);

  $transport->close();

} catch (TException $tx) {
  print 'TException: '.$tx->getMessage()."\n";
}

?>
