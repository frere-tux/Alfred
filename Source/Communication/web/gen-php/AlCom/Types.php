<?php
namespace AlCom;

/**
 * Autogenerated by Thrift Compiler (0.9.3)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
use Thrift\Base\TBase;
use Thrift\Type\TType;
use Thrift\Type\TMessageType;
use Thrift\Exception\TException;
use Thrift\Exception\TProtocolException;
use Thrift\Protocol\TProtocol;
use Thrift\Protocol\TBinaryProtocolAccelerated;
use Thrift\Exception\TApplicationException;


/**
 * Structs are the basic complex data structures. They are comprised of fields
 * which each have an integer identifier, a type, a symbolic name, and an
 * optional default value.
 * 
 * Fields can be declared "optional", which ensures they will not be included
 * in the serialized output if they aren't set.  Note that this requires some
 * manual management in some languages.
 */
class SimpleRequest {
  static $_TSPEC;

  /**
   * @var int
   */
  public $interID = null;
  /**
   * @var bool
   */
  public $state = null;
  /**
   * @var bool
   */
  public $group = null;

  public function __construct($vals=null) {
    if (!isset(self::$_TSPEC)) {
      self::$_TSPEC = array(
        1 => array(
          'var' => 'interID',
          'type' => TType::I32,
          ),
        2 => array(
          'var' => 'state',
          'type' => TType::BOOL,
          ),
        3 => array(
          'var' => 'group',
          'type' => TType::BOOL,
          ),
        );
    }
    if (is_array($vals)) {
      if (isset($vals['interID'])) {
        $this->interID = $vals['interID'];
      }
      if (isset($vals['state'])) {
        $this->state = $vals['state'];
      }
      if (isset($vals['group'])) {
        $this->group = $vals['group'];
      }
    }
  }

  public function getName() {
    return 'SimpleRequest';
  }

  public function read($input)
  {
    $xfer = 0;
    $fname = null;
    $ftype = 0;
    $fid = 0;
    $xfer += $input->readStructBegin($fname);
    while (true)
    {
      $xfer += $input->readFieldBegin($fname, $ftype, $fid);
      if ($ftype == TType::STOP) {
        break;
      }
      switch ($fid)
      {
        case 1:
          if ($ftype == TType::I32) {
            $xfer += $input->readI32($this->interID);
          } else {
            $xfer += $input->skip($ftype);
          }
          break;
        case 2:
          if ($ftype == TType::BOOL) {
            $xfer += $input->readBool($this->state);
          } else {
            $xfer += $input->skip($ftype);
          }
          break;
        case 3:
          if ($ftype == TType::BOOL) {
            $xfer += $input->readBool($this->group);
          } else {
            $xfer += $input->skip($ftype);
          }
          break;
        default:
          $xfer += $input->skip($ftype);
          break;
      }
      $xfer += $input->readFieldEnd();
    }
    $xfer += $input->readStructEnd();
    return $xfer;
  }

  public function write($output) {
    $xfer = 0;
    $xfer += $output->writeStructBegin('SimpleRequest');
    if ($this->interID !== null) {
      $xfer += $output->writeFieldBegin('interID', TType::I32, 1);
      $xfer += $output->writeI32($this->interID);
      $xfer += $output->writeFieldEnd();
    }
    if ($this->state !== null) {
      $xfer += $output->writeFieldBegin('state', TType::BOOL, 2);
      $xfer += $output->writeBool($this->state);
      $xfer += $output->writeFieldEnd();
    }
    if ($this->group !== null) {
      $xfer += $output->writeFieldBegin('group', TType::BOOL, 3);
      $xfer += $output->writeBool($this->group);
      $xfer += $output->writeFieldEnd();
    }
    $xfer += $output->writeFieldStop();
    $xfer += $output->writeStructEnd();
    return $xfer;
  }

}


