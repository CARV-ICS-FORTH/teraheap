
###################################################
#
# file: Makefile
#
# @Author:   Iacovos G. Kolokasis
# @Version:  20-09-2020
# @email:    kolokasis@ics.forth.gr
#
# Makefile for test files
#
####################################################

JAVA_REPO := $(dir $(abspath $(firstword $(MAKEFILE_LIST))))../../jdk8u345/
JAVAC=$(JAVA_REPO)/build/linux-x86_64-normal-server-release/jdk/bin/javac
GDB=gdb

TARGET=Array.class Array_List.class Array_List_Int.class List_Large.class \
	   MultiList.class Simple_Lambda.class Extend_Lambda.class \
	   Test_Reflection.class Test_Reference.class HashMap.class Rehashing.class \
	   Clone.class Groupping.class MultiHashMap.class Test_WeakHashMap.class \
	   ClassInstance.class
	
all: $(TARGET)

Array.class: Array.java
	$(JAVAC) $<

Array_List.class: Array_List.java 
	$(JAVAC) $<

Array_List_Int.class: Array_List_Int.java 
	$(JAVAC) $<

List_Large.class: List_Large.java 
	$(JAVAC) $<

MultiList.class: MultiList.java 
	$(JAVAC) $<

Simple_Lambda.class: Simple_Lambda.java 
	$(JAVAC) $<

Extend_Lambda.class: Extend_Lambda.java 
	$(JAVAC) $<

Test_Reflection.class: Test_Reflection.java
	$(JAVAC) $<

Test_Reference.class: Test_Reference.java 
	$(JAVAC) $<

HashMap.class: HashMap.java 
	$(JAVAC) -g $<

Rehashing.class: Rehashing.java
	$(JAVAC) $<

Clone.class: Clone.java
	$(JAVAC) $<

Groupping.class: Groupping.java
	$(JAVAC) $<

MultiHashMap.class: MultiHashMap.java
	$(JAVAC) $<

Test_WeakHashMap.class: Test_WeakHashMap.java
	$(JAVAC) $<

ClassInstance.class: ClassInstance.java
	$(JAVAC) $<

clean:
	-rm core.*
	-rm hs_err_pid*
	-rm err out
	-rm hotspot_pid*
	-rm replay_pid*
	-rm llarge_teraCache.txt

distclean: clean
	-rm -f *.class
