; Auto-generated. Do not edit!


(cl:in-package ros_msg-msg)


;//! \htmlinclude msgData.msg.html

(cl:defclass <msgData> (roslisp-msg-protocol:ros-message)
  ((Join_data
    :reader Join_data
    :initarg :Join_data
    :type (cl:vector cl:float)
   :initform (cl:make-array 6 :element-type 'cl:float :initial-element 0.0))
   (Join_state
    :reader Join_state
    :initarg :Join_state
    :type (cl:vector cl:float)
   :initform (cl:make-array 6 :element-type 'cl:float :initial-element 0.0))
   (data
    :reader data
    :initarg :data
    :type cl:integer
    :initform 0))
)

(cl:defclass msgData (<msgData>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <msgData>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'msgData)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name ros_msg-msg:<msgData> is deprecated: use ros_msg-msg:msgData instead.")))

(cl:ensure-generic-function 'Join_data-val :lambda-list '(m))
(cl:defmethod Join_data-val ((m <msgData>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader ros_msg-msg:Join_data-val is deprecated.  Use ros_msg-msg:Join_data instead.")
  (Join_data m))

(cl:ensure-generic-function 'Join_state-val :lambda-list '(m))
(cl:defmethod Join_state-val ((m <msgData>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader ros_msg-msg:Join_state-val is deprecated.  Use ros_msg-msg:Join_state instead.")
  (Join_state m))

(cl:ensure-generic-function 'data-val :lambda-list '(m))
(cl:defmethod data-val ((m <msgData>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader ros_msg-msg:data-val is deprecated.  Use ros_msg-msg:data instead.")
  (data m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <msgData>) ostream)
  "Serializes a message object of type '<msgData>"
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let ((bits (roslisp-utils:encode-double-float-bits ele)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream)))
   (cl:slot-value msg 'Join_data))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let ((bits (roslisp-utils:encode-double-float-bits ele)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream)))
   (cl:slot-value msg 'Join_state))
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'data)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'data)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'data)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'data)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <msgData>) istream)
  "Deserializes a message object of type '<msgData>"
  (cl:setf (cl:slot-value msg 'Join_data) (cl:make-array 6))
  (cl:let ((vals (cl:slot-value msg 'Join_data)))
    (cl:dotimes (i 6)
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:aref vals i) (roslisp-utils:decode-double-float-bits bits)))))
  (cl:setf (cl:slot-value msg 'Join_state) (cl:make-array 6))
  (cl:let ((vals (cl:slot-value msg 'Join_state)))
    (cl:dotimes (i 6)
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:aref vals i) (roslisp-utils:decode-double-float-bits bits)))))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'data)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'data)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'data)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'data)) (cl:read-byte istream))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<msgData>)))
  "Returns string type for a message object of type '<msgData>"
  "ros_msg/msgData")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'msgData)))
  "Returns string type for a message object of type 'msgData"
  "ros_msg/msgData")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<msgData>)))
  "Returns md5sum for a message object of type '<msgData>"
  "73f187edf478e820d25171010c16a164")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'msgData)))
  "Returns md5sum for a message object of type 'msgData"
  "73f187edf478e820d25171010c16a164")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<msgData>)))
  "Returns full string definition for message of type '<msgData>"
  (cl:format cl:nil "float64[6] Join_data~%float64[6] Join_state~%uint32 data~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'msgData)))
  "Returns full string definition for message of type 'msgData"
  (cl:format cl:nil "float64[6] Join_data~%float64[6] Join_state~%uint32 data~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <msgData>))
  (cl:+ 0
     0 (cl:reduce #'cl:+ (cl:slot-value msg 'Join_data) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 8)))
     0 (cl:reduce #'cl:+ (cl:slot-value msg 'Join_state) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 8)))
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <msgData>))
  "Converts a ROS message object to a list"
  (cl:list 'msgData
    (cl:cons ':Join_data (Join_data msg))
    (cl:cons ':Join_state (Join_state msg))
    (cl:cons ':data (data msg))
))
